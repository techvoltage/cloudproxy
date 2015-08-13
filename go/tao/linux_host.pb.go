// Code generated by protoc-gen-go.
// source: linux_host.proto
// DO NOT EDIT!

package tao

import proto "github.com/golang/protobuf/proto"
import math "math"

// Reference imports to suppress errors if they are not otherwise used.
var _ = proto.Marshal
var _ = math.Inf

type LinuxHostSealedBundle struct {
	Policy           *string `protobuf:"bytes,1,req,name=policy" json:"policy,omitempty"`
	PolicyInfo       *string `protobuf:"bytes,2,opt,name=policy_info" json:"policy_info,omitempty"`
	Data             []byte  `protobuf:"bytes,3,req,name=data" json:"data,omitempty"`
	XXX_unrecognized []byte  `json:"-"`
}

func (m *LinuxHostSealedBundle) Reset()         { *m = LinuxHostSealedBundle{} }
func (m *LinuxHostSealedBundle) String() string { return proto.CompactTextString(m) }
func (*LinuxHostSealedBundle) ProtoMessage()    {}

func (m *LinuxHostSealedBundle) GetPolicy() string {
	if m != nil && m.Policy != nil {
		return *m.Policy
	}
	return ""
}

func (m *LinuxHostSealedBundle) GetPolicyInfo() string {
	if m != nil && m.PolicyInfo != nil {
		return *m.PolicyInfo
	}
	return ""
}

func (m *LinuxHostSealedBundle) GetData() []byte {
	if m != nil {
		return m.Data
	}
	return nil
}
