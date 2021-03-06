// Code generated by protoc-gen-go.
// source: rpc.proto
// DO NOT EDIT!

/*
Package tao is a generated protocol buffer package.

It is generated from these files:
	rpc.proto

It has these top-level messages:
	RPCRequest
	RPCResponse
*/
package tao

import proto "github.com/golang/protobuf/proto"
import fmt "fmt"
import math "math"

// Reference imports to suppress errors if they are not otherwise used.
var _ = proto.Marshal
var _ = fmt.Errorf
var _ = math.Inf

type RPCRequest struct {
	Data             []byte  `protobuf:"bytes,1,opt,name=data" json:"data,omitempty"`
	Size             *int32  `protobuf:"varint,2,opt,name=size" json:"size,omitempty"`
	Policy           *string `protobuf:"bytes,3,opt,name=policy" json:"policy,omitempty"`
	Time             *int64  `protobuf:"varint,4,opt,name=time" json:"time,omitempty"`
	Expiration       *int64  `protobuf:"varint,5,opt,name=expiration" json:"expiration,omitempty"`
	Issuer           []byte  `protobuf:"bytes,6,opt,name=issuer" json:"issuer,omitempty"`
	Label            *string `protobuf:"bytes,7,opt,name=label" json:"label,omitempty"`
	Counter          *int64  `protobuf:"varint,8,opt,name=counter" json:"counter,omitempty"`
	XXX_unrecognized []byte  `json:"-"`
}

func (m *RPCRequest) Reset()                    { *m = RPCRequest{} }
func (m *RPCRequest) String() string            { return proto.CompactTextString(m) }
func (*RPCRequest) ProtoMessage()               {}
func (*RPCRequest) Descriptor() ([]byte, []int) { return fileDescriptor0, []int{0} }

func (m *RPCRequest) GetData() []byte {
	if m != nil {
		return m.Data
	}
	return nil
}

func (m *RPCRequest) GetSize() int32 {
	if m != nil && m.Size != nil {
		return *m.Size
	}
	return 0
}

func (m *RPCRequest) GetPolicy() string {
	if m != nil && m.Policy != nil {
		return *m.Policy
	}
	return ""
}

func (m *RPCRequest) GetTime() int64 {
	if m != nil && m.Time != nil {
		return *m.Time
	}
	return 0
}

func (m *RPCRequest) GetExpiration() int64 {
	if m != nil && m.Expiration != nil {
		return *m.Expiration
	}
	return 0
}

func (m *RPCRequest) GetIssuer() []byte {
	if m != nil {
		return m.Issuer
	}
	return nil
}

func (m *RPCRequest) GetLabel() string {
	if m != nil && m.Label != nil {
		return *m.Label
	}
	return ""
}

func (m *RPCRequest) GetCounter() int64 {
	if m != nil && m.Counter != nil {
		return *m.Counter
	}
	return 0
}

type RPCResponse struct {
	Data             []byte  `protobuf:"bytes,1,opt,name=data" json:"data,omitempty"`
	Policy           *string `protobuf:"bytes,2,opt,name=policy" json:"policy,omitempty"`
	Counter          *int64  `protobuf:"varint,3,opt,name=counter" json:"counter,omitempty"`
	XXX_unrecognized []byte  `json:"-"`
}

func (m *RPCResponse) Reset()                    { *m = RPCResponse{} }
func (m *RPCResponse) String() string            { return proto.CompactTextString(m) }
func (*RPCResponse) ProtoMessage()               {}
func (*RPCResponse) Descriptor() ([]byte, []int) { return fileDescriptor0, []int{1} }

func (m *RPCResponse) GetData() []byte {
	if m != nil {
		return m.Data
	}
	return nil
}

func (m *RPCResponse) GetPolicy() string {
	if m != nil && m.Policy != nil {
		return *m.Policy
	}
	return ""
}

func (m *RPCResponse) GetCounter() int64 {
	if m != nil && m.Counter != nil {
		return *m.Counter
	}
	return 0
}

func init() {
	proto.RegisterType((*RPCRequest)(nil), "tao.RPCRequest")
	proto.RegisterType((*RPCResponse)(nil), "tao.RPCResponse")
}

/*
var fileDescriptor0 = []byte{
	// 180 bytes of a gzipped FileDescriptorProto
	0x1f, 0x8b, 0x08, 0x00, 0x00, 0x09, 0x6e, 0x88, 0x02, 0xff, 0x5c, 0x8e, 0x3d, 0x8e, 0xc2, 0x30,
	0x10, 0x85, 0x95, 0x38, 0x3f, 0x9b, 0xd9, 0xec, 0xae, 0xe4, 0x6a, 0xca, 0x55, 0x2a, 0x2a, 0x4e,
	0x40, 0xc7, 0x05, 0x50, 0x6e, 0x60, 0xc2, 0x14, 0x96, 0x42, 0xc6, 0xd8, 0x13, 0x09, 0x38, 0x02,
	0xa7, 0xc6, 0x76, 0x83, 0x94, 0xf2, 0xf3, 0xf3, 0x7b, 0xf3, 0x41, 0xe7, 0xdd, 0xb4, 0x77, 0x9e,
	0x85, 0xb5, 0x12, 0xc3, 0xc3, 0xab, 0x00, 0x18, 0x4f, 0xc7, 0x91, 0x6e, 0x2b, 0x05, 0xd1, 0x3d,
	0x54, 0x17, 0x23, 0x06, 0x8b, 0xff, 0x62, 0xd7, 0x27, 0x0a, 0xf6, 0x49, 0x58, 0x46, 0xaa, 0xf5,
	0x2f, 0x34, 0x8e, 0x67, 0x3b, 0x3d, 0x50, 0x45, 0xee, 0x52, 0x2a, 0xf6, 0x4a, 0x58, 0x45, 0x52,
	0x5a, 0x03, 0xd0, 0xdd, 0x59, 0x6f, 0xc4, 0xf2, 0x82, 0x75, 0x7e, 0x8b, 0x0d, 0x1b, 0xc2, 0x4a,
	0x1e, 0x9b, 0xbc, 0xf7, 0x03, 0xf5, 0x6c, 0xce, 0x34, 0x63, 0x9b, 0x07, 0xfe, 0xa0, 0x9d, 0x78,
	0x5d, 0x24, 0xe6, 0x5f, 0xe9, 0xff, 0x70, 0x80, 0xef, 0xec, 0x12, 0x1c, 0x2f, 0x81, 0x36, 0x32,
	0x9f, 0xf3, 0xe5, 0xb6, 0x9d, 0x7c, 0xd4, 0x3b, 0x00, 0x00, 0xff, 0xff, 0x05, 0x51, 0xc8, 0x46,
	0xdb, 0x00, 0x00, 0x00,
}
*/
