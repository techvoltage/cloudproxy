// Code generated by protoc-gen-go.
// source: datalog_guard.proto
// DO NOT EDIT!

package tao

import proto "code.google.com/p/goprotobuf/proto"
import math "math"

// Reference imports to suppress errors if they are not otherwise used.
var _ = proto.Marshal
var _ = math.Inf

// A set of rules.
type DatalogRules struct {
	Rules            []string `protobuf:"bytes,1,rep,name=rules" json:"rules,omitempty"`
	XXX_unrecognized []byte   `json:"-"`
}

func (m *DatalogRules) Reset()         { *m = DatalogRules{} }
func (m *DatalogRules) String() string { return proto.CompactTextString(m) }
func (*DatalogRules) ProtoMessage()    {}

func (m *DatalogRules) GetRules() []string {
	if m != nil {
		return m.Rules
	}
	return nil
}

// A set of rules signed by a key.
type SignedDatalogRules struct {
	SerializedRules  []byte `protobuf:"bytes,1,req,name=serialized_rules" json:"serialized_rules,omitempty"`
	Signature        []byte `protobuf:"bytes,2,req,name=signature" json:"signature,omitempty"`
	XXX_unrecognized []byte `json:"-"`
}

func (m *SignedDatalogRules) Reset()         { *m = SignedDatalogRules{} }
func (m *SignedDatalogRules) String() string { return proto.CompactTextString(m) }
func (*SignedDatalogRules) ProtoMessage()    {}

func (m *SignedDatalogRules) GetSerializedRules() []byte {
	if m != nil {
		return m.SerializedRules
	}
	return nil
}

func (m *SignedDatalogRules) GetSignature() []byte {
	if m != nil {
		return m.Signature
	}
	return nil
}

func init() {
}
