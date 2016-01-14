// Copyright (c) 2015, Google, Inc. All rights reserved.
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
//

package main

import (
	"crypto/rsa"
	"crypto/x509"
	"errors"
	"flag"
	"fmt"
	"io"
	"io/ioutil"

	"github.com/golang/protobuf/proto"
	"github.com/jlmucb/cloudproxy/src/tpm2/tpm20"
)

// return handle, policy digest
func assistCreateSession(rw io.ReadWriteCloser, hash_alg uint16) (tpm.Handle, []byte, error) {
	nonceCaller := []byte{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	var secret []byte
	sym := uint16(tpm.AlgTPM_ALG_NULL)

	session_handle, policy_digest, err := tpm.StartAuthSession(rw,
		tpm.Handle(tpm.OrdTPM_RH_NULL),
		tpm.Handle(tpm.OrdTPM_RH_NULL), nonceCaller, secret,
		uint8(tpm.OrdTPM_SE_POLICY), sym, hash_alg)
	if err != nil {
		return tpm.Handle(0), nil, errors.New("Can't start session")
	}
	fmt.Printf("policy digest  : %x\n", policy_digest)

	err = tpm.PolicyPassword(rw, session_handle)
	if err != nil {
		fmt.Printf("PolicyPcr fails")
		return tpm.Handle(0), nil, errors.New("Can't set policy password")
	}
	var tpm_digest []byte
	err = tpm.PolicyPcr(rw, session_handle, tpm_digest, []int{7})
	if err != nil {
		fmt.Printf("PolicyPcr fails")
		return tpm.Handle(0), nil, errors.New("Can't set policy pcr")
	}

	policy_digest, err = tpm.PolicyGetDigest(rw, session_handle)
	if err != nil {
		fmt.Printf("PolicyGetDigest after PolicyPcr fails")
		return tpm.Handle(0), nil, errors.New("Can't start session")
	}
	fmt.Printf("policy digest after PolicyPcr: %x\n", policy_digest)
	return session_handle, policy_digest, nil
}

// out: private, public
func assistSeal(rw io.ReadWriteCloser, parentHandle tpm.Handle, toSeal []byte,
	parentPassword string, ownerPassword string,
	pcrs[]int, policy_digest []byte) ([]byte, []byte, error) {

	var empty []byte
	keyedhashparms := tpm.KeyedHashParams{uint16(tpm.AlgTPM_ALG_KEYEDHASH), uint16(tpm.AlgTPM_ALG_SHA1),
		uint32(0x00000012), empty, uint16(tpm.AlgTPM_ALG_AES), uint16(128),
		uint16(tpm.AlgTPM_ALG_CFB), uint16(tpm.AlgTPM_ALG_NULL), empty}
	private_blob, public_blob, err := tpm.CreateSealed(rw, parentHandle, policy_digest,
		parentPassword,  ownerPassword, toSeal, []int{7}, keyedhashparms)
	if err != nil {
		return nil, nil, nil
	}
	return private_blob, public_blob, nil
}

// out: unsealed blob, nonce
func assistUnseal(rw io.ReadWriteCloser, sessionHandle tpm.Handle, primaryHandle tpm.Handle,
	pub []byte, priv []byte, parentPassword string, ownerPassword string,
	policy_digest []byte) ([]byte, []byte, error) {

	// Load Sealed
	sealHandle, _, err := tpm.Load(rw, primaryHandle, parentPassword, ownerPassword,
		pub, priv)
	if err != nil {
		fmt.Printf("Load fails\n")
		return nil, nil, errors.New("Load failed")
	}
	fmt.Printf("Load succeeded\n")

	// Unseal
	unsealed, nonce, err := tpm.Unseal(rw, sealHandle, ownerPassword,
		sessionHandle, policy_digest)
	if err != nil {
		fmt.Printf("Unseal fails\n")
		return nil, nil, errors.New("Unseal failed")
	}
	return unsealed, nonce, err
}

// This program runs the cloudproxy protocol.
func main() {
	keySize := flag.Int("modulus size",  2048,
		"Modulus size for keys")
	hashAlg := flag.String("hash algorithm",  "sha1",
		"hash algorithm used")
	permPrimaryHandle := flag.Uint("primary handle", 0x810003e8,
		"permenant primary handle")
	permQuoteHandle := flag.Uint("quote handle", 0x810003e9,
		"permenant quote handle")
	fileNameEndorsementCert := flag.String("Endorsement cert file",
		"../tmptest/endorsement_cert", "endorsement cert")
	fileNamePolicyCert := flag.String("Policy cert",
		"../tmptest/policy_key_cert", "policy_key_cert")
	fileNamePolicyKey := flag.String("Policy key",
		"../tmptest/cloudproxy_key_file.proto", "policy_key_cert")
	fileNameSigningInstructions := flag.String("Signing instructions",
		"../tmptest/signing_instructions", "signing instructions")
	quoteOwnerPassword := flag.String("Quote owner password", "01020304",
		"quote owner password")
	sealedOwnerPassword := flag.String("Sealed owner password", "01020304",
		"sealed owner password")
	sealedParentPassword := flag.String("Sealed parent password", "01020304",
		"sealed parent password")
	sealedProgramKeyFile := flag.String("Sealed program key file", "../tmptest/test_program_key",
		"sealed program key file")
	programCertFile := flag.String("Program cert file", "../tmptest/test_program_key.cert",
		"sealed program key file")
	programName := flag.String("Application program name", "TestProgram",
		"program name")
	flag.Parse()

	fmt.Printf("Primary handle: %x, quote handle: %x\n",
		*permPrimaryHandle, *permQuoteHandle)
	fmt.Printf("Endorsement cert file: %s, Policy cert file: %s, Policy key file: %s\n",
		*fileNameEndorsementCert, *fileNamePolicyCert, *fileNamePolicyKey)
	fmt.Printf("Program name: %s, Signing Instructions file: %s\n",
		*programName, *fileNameSigningInstructions)
	fmt.Printf("modulus size: %d,  hash algorithm: %s\n",
		*keySize, *hashAlg)
	fmt.Printf("sealedParent password: %s,  sealedOwner password : %s, sealed key file: %s\n",
		*sealedParentPassword, *sealedOwnerPassword, *sealedProgramKeyFile)
	fmt.Printf("Program key cert: %s\n", *programCertFile)

	// Read Endorsement key info
	derEndorsementCert := tpm.RetrieveFile(*fileNameEndorsementCert)
	if derEndorsementCert == nil {
		fmt.Printf("Can't read endorsement cert\n")
		return
	}
	// Get endorsement public from cert
	endorsement_cert, err := x509.ParseCertificate(derEndorsementCert)
	if err != nil {
		fmt.Printf("Endorsement ParseCertificate fails\n")
		return
	}
	fmt.Printf("Endorsement cert: %x\n", derEndorsementCert)

	var protectorPublic *rsa.PublicKey
	switch k :=  endorsement_cert.PublicKey.(type) {
	case  *rsa.PublicKey:
		protectorPublic = k
	case  *rsa.PrivateKey:
		protectorPublic = &k.PublicKey
	default:
		fmt.Printf("endorsement cert is not an rsa key\n")
		return
	}
	fmt.Printf("Endorsement publix: %x\n", protectorPublic)

	// Open tpm
	rw, err := tpm.OpenTPM("/dev/tpm0")
	if err != nil {
		fmt.Printf("OpenTPM failed %s\n", err)
		return
	}
	defer rw.Close()

	// CreatePrimary
	var empty []byte
	primaryparms := tpm.RsaParams{uint16(tpm.AlgTPM_ALG_RSA),
		uint16(tpm.AlgTPM_ALG_SHA1), uint32(0x00030072), empty,
		uint16(tpm.AlgTPM_ALG_AES), uint16(128),
		uint16(tpm.AlgTPM_ALG_CFB), uint16(tpm.AlgTPM_ALG_NULL),
		uint16(0), 2048, uint32(0x00010001), empty}
	protectorHandle, _, err := tpm.CreatePrimary(rw,
		uint32(tpm.OrdTPM_RH_ENDORSEMENT), []int{0x7}, "", "", primaryparms)
	if err != nil {
		fmt.Printf("CreatePrimary fails")
		return
	}
	fmt.Printf("CreatePrimary succeeded\n")
	fmt.Printf("Endorsement handle: %x\n", protectorHandle)

	// Read Policy cert
	derPolicyCert := tpm.RetrieveFile(*fileNamePolicyCert)
	if derPolicyCert == nil {
		fmt.Printf("Can't read policy cert\n")
		return
	}

	// Read Policy key
	protoPolicyKey := tpm.RetrieveFile(*fileNamePolicyKey)
	if protoPolicyKey == nil {
		fmt.Printf("Can't read policy key file\n")
		return
	}

	// Parse policy key
	keyMsg := new(tpm.RsaPrivateKeyMessage)
	err = proto.Unmarshal(protoPolicyKey, keyMsg)
	if err != nil {
		fmt.Printf("Can't unmarshal policy key\n")
		return
	}
	policyPrivateKey, err := tpm.UnmarshalRsaPrivateFromProto(keyMsg)
	if err != nil {
		fmt.Printf("Can't decode policy key\n")
		return
	}
	fmt.Printf("Key: %x\n", policyPrivateKey)

	// Read signing instructions
	signingInstructionsIn := tpm.RetrieveFile(*fileNameSigningInstructions)
	if signingInstructionsIn == nil {
		fmt.Printf("Can't read signing instructions\n")
		return
	}
	signing_instructions_message := new(tpm.SigningInstructionsMessage)
	err = proto.Unmarshal(signingInstructionsIn,
		signing_instructions_message)
	if  err != nil {
		fmt.Printf("Can't unmarshal signing instructions\n", err)
		return
	}
	fmt.Printf("Got signing instructions\n")

	// Protocol
	fmt.Printf("Program name is %s\n",  *programName)
	prog_name := *programName
	protoClientPrivateKey, request, err := tpm.ConstructClientRequest(rw,
		derEndorsementCert, tpm.Handle(*permQuoteHandle), "", *quoteOwnerPassword,
		prog_name)
	if err != nil {
		fmt.Printf("ConstructClientRequest failed\n")
		return
	}
	fmt.Printf("ConstructClientRequest succeeded\n")
	fmt.Printf("Key: %s\n", proto.CompactTextString(protoClientPrivateKey))
	fmt.Printf("Request: %s\n", proto.CompactTextString(request))
	fmt.Printf("Program name from request: %s\n", *request.ProgramKey.ProgramName)

/*
	// Create Session for seal/unseal
	sessionHandle, policy_digest, err := assistCreateSession(rw, tpm.AlgTPM_ALG_SHA1)
	if err != nil {
		fmt.Printf("Can't start session for Seal\n")
		return
	}

	// Serialize the client private key proto, seal it and save it.
	serializedProgramKey := proto.CompactTextString(protoClientPrivateKey)
	sealed_priv, sealed_pub, err := assistSeal(rw, tpm.Handle(*permPrimaryHandle),
		[]byte(serializedProgramKey), *sealedParentPassword, *sealedOwnerPassword,
		[]int{7}, policy_digest)
	if err != nil {
		fmt.Printf("Can't seal Program private key\n")
		return
		}
	ioutil.WriteFile(*sealedProgramKeyFile + ".private", sealed_priv, 0644)
	ioutil.WriteFile(*sealedProgramKeyFile + ".public", sealed_pub, 0644)
*/

	response, err := tpm.ConstructServerResponse(policyPrivateKey, *signing_instructions_message,
		*request)
	if err != nil {
		fmt.Printf("ConstructServerResponse failed\n")
		return
	}
	if response == nil {
		fmt.Printf("response is nil\n")
	}
	fmt.Printf("Response for ProgramName %s\n", *response.ProgramName)
	cert, err := tpm.ClientDecodeServerResponse(rw, protectorHandle,
                tpm.Handle(*permQuoteHandle), *quoteOwnerPassword, *response)
	if err != nil {
		fmt.Printf("ClientDecodeServerResponse failed\n")
		return
	}

/*
	// recover program private key
	programPrivateBlob := tpm.RetrieveFile(*sealedProgramKeyFile + ".private")
	programPublicBlob := tpm.RetrieveFile(*sealedProgramKeyFile + ".public")
	unsealed, _, err := assistUnseal(rw, sessionHandle, tpm.Handle(*permPrimaryHandle),
		programPublicBlob, programPrivateBlob, "", "", policy_digest)

	newPrivKeyMsg := new(tpm.RsaPrivateKeyMessage)
        err = proto.Unmarshal(unsealed, newPrivKeyMsg)
        newProgramKey, err := tpm.UnmarshalRsaPrivateFromProto(newPrivKeyMsg)
        if err != nil {
                fmt.Printf("Can't unmarshal key to proto\n")
        }	
	fmt.Printf("Recovered Program keys: %x\n", newProgramKey)
 */

	// Save cert 
	fmt.Printf("Client cert: %x\n", cert)
	ioutil.WriteFile(*programCertFile, cert, 0644)

	fmt.Printf("Cloudproxy protocol succeeds\n")
	return
}
