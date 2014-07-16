package util

import (
	"code.google.com/p/goprotobuf/proto"
	"encoding/binary"
	"errors"
	"io"
	"math"
)

// A MessageStream is an io.ReadWriteCloser that can also read and write strings
// and protobuf messages. Boundaries are preserved for strings and protobuf
// messages using a 32-bit (network byte order) length prefix before the
// contents of the string or marshalled protobuf message. MessageStream can also
// enforce an upper-limit on the size of received messages.
type MessageStream struct {
	MaxMessageSize int // Negative means unlimited
	io.ReadWriteCloser
}

const DefaultMaxMessageSize = 20 * 1024 * 1024

var ErrMessageTooLarge = errors.New("messagestream: Message is too large")

// WriteString writes a 32-bit length followed by the string.
func (ms *MessageStream) WriteString(s string) (int, error) {
	n := len(s)
	if n > math.MaxUint32 {
		return 0, ErrMessageTooLarge
	}
	var sizebytes [4]byte
	binary.BigEndian.PutUint32(sizebytes[:], uint32(n))
	n, err := ms.Write(sizebytes[:])
	if err != nil {
		return n, err
	}
	m, err := ms.Write([]byte(s))
	return n+m, err
}

// ReadString reads a 32-bit length followed by a string.
func (ms *MessageStream) ReadString() (string, error) {
	var sizebytes [4]byte
	_, err := io.ReadFull(ms, sizebytes[:])
	if err != nil {
		return "", err
	}
	n := binary.BigEndian.Uint32(sizebytes[:])
	max := ms.MaxMessageSize
	// We also check for int(n) to overflow so allocation below doesn't fail.
	if int(n) < 0 || (max > 0 && int(n) > max) {
		return "", ErrMessageTooLarge
	}
	strbytes := make([]byte, int(n))
	_, err = io.ReadFull(ms, strbytes)
	if err != nil {
		return "", err
	}
	return string(strbytes), nil
}

// WriteMessage writes 32-bit length followed by a protobuf message.
func (ms *MessageStream) WriteMessage(m proto.Message) (int, error) {
	bytes, err := proto.Marshal(m)
	if err != nil {
		return 0, err
	}
	return ms.WriteString(string(bytes))
}

// ReadMessage reads a 32-bit length followed by a protobuf message.
func (ms *MessageStream) ReadMessage(m proto.Message) error {
	s, err := ms.ReadString()
	if err != nil {
		return err
	}
	err = proto.Unmarshal([]byte(s), m)
	if err != nil {
		return err
	}
	return nil
}

// NewMessageStream creates a MessageStream for the given pipe with a reception
// limit of DefaultMaxMessageSize..
func NewMessageStream(pipe io.ReadWriteCloser) *MessageStream {
	return &MessageStream{DefaultMaxMessageSize, pipe}
}
