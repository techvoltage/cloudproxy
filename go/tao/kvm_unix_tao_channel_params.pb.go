// Code generated by protoc-gen-go.
// source: kvm_unix_tao_channel_params.proto
// DO NOT EDIT!

package tao

import proto "code.google.com/p/goprotobuf/proto"
import math "math"

// Reference imports to suppress errors if they are not otherwise used.
var _ = proto.Marshal
var _ = math.Inf

type KvmUnixTaoChannelParams struct {
	GuestDevice      *string `protobuf:"bytes,1,req,name=guest_device" json:"guest_device,omitempty"`
	HostDevice       *string `protobuf:"bytes,2,opt,name=host_device" json:"host_device,omitempty"`
	XXX_unrecognized []byte  `json:"-"`
}

func (m *KvmUnixTaoChannelParams) Reset()         { *m = KvmUnixTaoChannelParams{} }
func (m *KvmUnixTaoChannelParams) String() string { return proto.CompactTextString(m) }
func (*KvmUnixTaoChannelParams) ProtoMessage()    {}

func (m *KvmUnixTaoChannelParams) GetGuestDevice() string {
	if m != nil && m.GuestDevice != nil {
		return *m.GuestDevice
	}
	return ""
}

func (m *KvmUnixTaoChannelParams) GetHostDevice() string {
	if m != nil && m.HostDevice != nil {
		return *m.HostDevice
	}
	return ""
}

func init() {
}
