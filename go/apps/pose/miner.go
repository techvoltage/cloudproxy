// Copyright (c) 2016, Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package pose

import (
	"crypto"
	"crypto/ecdsa"
	"crypto/elliptic"
	"crypto/rand"
	"crypto/sha256"
	"crypto/tls"
	"crypto/x509"
	"crypto/x509/pkix"
	"encoding/binary"
	"fmt"
	"log"
	"net"
	"net/rpc"
	"strings"
	"sync"
	"time"

	"github.com/golang/protobuf/proto"
	"github.com/jlmucb/cloudproxy/go/tao"
)

type Miner struct {
	addr    string
	network string

	peers    []string
	peerRPCs []*rpc.Client

	block chan *Block

	keys     *tao.Keys // Signing keys of this hosted program.
	signer   crypto.Signer
	domain   *tao.Domain  // Policy guard and public key.
	listener net.Listener // Socket where server listens for proxies/routers
	t        tao.Tao

	// The data is probably submitted from other users
	data     []byte
	dataLock *sync.Mutex

	// Previous sent blocks should be marked to avoid loop.
	sent     map[string]bool
	sentLock *sync.Mutex

	chain *Chain

	id int //for debugging

	difficulty  int
	confirmTime int

	kill chan bool
}

// Used to avoid warnings about having wrong number of ins/outs
type MinerRPC struct {
	miner *Miner
}

//TODO: save the keys used by the clients
func NewMiner(network, addr, domainPath string,
	x509Identity *pkix.Name, t tao.Tao, dbFile string) (*Miner, error) {
	port := addr
	if addr != "127.0.0.1:0" {
		port = ":" + strings.Split(addr, ":")[1]
	}

	// Generate keys and get attestation from parent.
	keys, err := tao.NewTemporaryTaoDelegatedKeys(tao.Signing|tao.Crypting, t)
	if err != nil {
		return nil, err
	}

	// Create a certificate.
	keys.Cert, err = keys.SigningKey.CreateSelfSignedX509(x509Identity)
	if err != nil {
		return nil, err
	}

	// Load domain from local configuration.
	domain, err := tao.LoadDomain(domainPath, nil)
	if err != nil {
		return nil, err
	}

	// Encode TLS certificate.
	cert, err := tao.EncodeTLSCert(keys)
	if err != nil {
		return nil, err
	}

	tlsConfig := &tls.Config{
		RootCAs:      x509.NewCertPool(),
		Certificates: []tls.Certificate{*cert},
		// TODO(kwonalbert): change this flag
		InsecureSkipVerify: true,
		ClientAuth:         tls.RequestClientCert,
	}

	listener, err := tao.Listen(network, port, tlsConfig,
		domain.Guard, domain.Keys.VerifyingKey, keys.Delegation)
	if err != nil {
		return nil, err
	}

	signer, err := ecdsa.GenerateKey(elliptic.P256(), rand.Reader)
	if err != nil {
		return nil, err
	}

	chain, err := NewChain(dbFile, DEFAULT_CONFIRM_TIME)
	if err != nil {
		return nil, err
	}

	c := &Miner{
		addr:    addr,
		network: network,

		// peers set explicitly using set peers

		block: make(chan *Block),

		keys:     keys,
		signer:   signer,
		domain:   domain,
		listener: listener,
		t:        t,

		chain: chain,

		sent:     make(map[string]bool),
		sentLock: new(sync.Mutex),

		dataLock: new(sync.Mutex),

		difficulty:  1000,
		confirmTime: 6,

		kill: make(chan bool),
	}

	return c, nil
}

func (m *Miner) Serve() error {
	s := rpc.NewServer()
	mrpc := &MinerRPC{m}
	err := s.RegisterName("Miner", mrpc)
	if err != nil {
		return err
	}
	for {
		conn, err := m.listener.Accept()
		if err != nil {
			return err
		}

		go s.ServeConn(conn)
	}
}

func (m *MinerRPC) SendBlock(bb []byte, _ *int) error {
	m.miner.sentLock.Lock()
	block := &Block{}
	proto.Unmarshal(bb, block)

	// someone else won the lottery
	if *block.Index > *m.miner.chain.MainBlock().Index {
		// fmt.Println(m.miner.id, "Received:", *block.Index, hash[:10])
		// TODO(kwonalbert): delete/update data field
		m.miner.block <- block
	}

	bb, err := proto.Marshal(block)
	if err != nil {
		return err
	}
	hash := sha256.Sum256(bb)

	// If this block has been sent before,
	// no need to do anything
	if _, ok := m.miner.sent[string(hash[:])]; !ok {
		m.miner.sent[string(hash[:])] = true
		m.miner.chain.AddBlock(block)
		go m.miner.sendAllPeers(bb)
	}

	m.miner.sentLock.Unlock()
	return nil
}

func (m *MinerRPC) Chain(_ *int, blocks *[]*Block) error {
	*blocks = make([]*Block, m.miner.chain.size)
	var err error
	for b := range *blocks {
		(*blocks)[b], err = m.miner.chain.Block(b)
		if err != nil {
			return err
		}
	}
	return nil
}

func (m *MinerRPC) SendData(data []byte, _ *int) error {
	m.miner.dataLock.Lock()
	m.miner.data = append(m.miner.data, data...)
	m.miner.dataLock.Unlock()
	return nil
}

func (m *Miner) SetPeers(peers []string) {
	// Close the connection first
	for _, client := range m.peerRPCs {
		if client != nil {
			client.Close()
		}
	}

	m.peers = peers
	m.dialAllPeers()
}

func (m *Miner) SetDifficulty(difficulty int) {
	m.difficulty = difficulty
}

func (m *Miner) Difficulty() int {
	return m.difficulty
}

func (m *Miner) SetConfirmTime(confirmTime int) {
	m.confirmTime = confirmTime
	m.chain.SetConfirmTime(confirmTime)
}

func (m *Miner) Close() {
	m.kill <- true
	m.chain.Close()
	if m.listener != nil {
		m.listener.Close()
	}
	for _, peer := range m.peerRPCs {
		if peer != nil {
			peer.Close()
		}
	}
}

func (m *Miner) Mine() error {
	creator, err := x509.MarshalPKIXPublicKey(m.signer.Public())
	if err != nil {
		return err
	}
	for {
		r, err := m.t.GetRandomBytes(64)
		if err != nil {
			return err
		}

		wait := ((binary.LittleEndian.Uint64(r) % uint64(m.difficulty)) / GRANULARITY) * GRANULARITY
		//fmt.Println(m.id, "sleeping for ", *m.chain.MainBlock().Index, wait)
		select {
		case <-m.kill:
			break
		case <-time.After(time.Duration(wait) * INCREMENT):
			// this client won the lottery
			mb, err := proto.Marshal(m.chain.MainBlock())
			if err != nil {
				return err
			}
			hash := sha256.Sum256(mb)
			prevBlock := make([]byte, len(hash))
			copy(prevBlock, hash[:])
			index := *m.chain.main.Index + 1
			difficulty := int64(m.Difficulty())
			block := &Block{
				PrevBlock:  prevBlock,
				Index:      &index,
				Difficulty: &difficulty,
				Creator:    creator,
				Data:       m.data,
			}

			// Since you are done with this data, clear it
			m.data = nil
			m.chain.AddBlock(block)

			bb, err := proto.Marshal(block)
			if err != nil {
				return err
			}
			hash = sha256.Sum256(bb)
			m.sent[string(hash[:])] = true
			fmt.Println(m.id, "sending block:", *block.Index, hash[:10])
			m.sendAllPeers(bb)
		case <-m.block:
			// Someone else won the lottery
			continue
		}
	}
}

func (m *Miner) sendAllPeers(bb []byte) {
	wg := new(sync.WaitGroup)
	for p, peer := range m.peers {
		wg.Add(1)
		go func(p int, peer string) {
			wg.Done()
			err := m.peerRPCs[p].Call("Miner.SendBlock", bb, nil)
			if err != nil {
				log.Fatal(err)
			}
		}(p, peer)
	}
	wg.Wait()
}

// Dial all peers in your list.
// It ignores peers you can't connect to
func (m *Miner) dialAllPeers() {
	m.peerRPCs = make([]*rpc.Client, len(m.peers))
	for p, peer := range m.peers {
		go func(p int, peer string) {
			conn, err := m.dialPeer(peer)
			if err != nil {
				log.Println(err)
			}
			m.peerRPCs[p] = rpc.NewClient(conn)
		}(p, peer)
	}
}

// dial a cloudproxy peer.
func (m *Miner) dialPeer(peer string) (net.Conn, error) {
	conn, err := tao.Dial(m.network, peer, m.domain.Guard, m.domain.Keys.VerifyingKey, m.keys)
	if err != nil {
		return nil, err
	}
	return conn, nil
}

func (m *Miner) sign(blob []byte) ([]byte, error) {
	return m.signer.Sign(rand.Reader, blob, crypto.SHA256)
}
