// Code generated by protoc-gen-go.
// source: service.proto
// DO NOT EDIT!

/*
Package domain_service is a generated protocol buffer package.

It is generated from these files:
	service.proto

It has these top-level messages:
	DomainServiceRequest
	DomainServiceResponse
	TrustedEntities
*/
package domain_service

import proto "github.com/golang/protobuf/proto"
import fmt "fmt"
import math "math"

// Reference imports to suppress errors if they are not otherwise used.
var _ = proto.Marshal
var _ = fmt.Errorf
var _ = math.Inf

// This is a compile-time assertion to ensure that this generated file
// is compatible with the proto package it is being compiled against.
// A compilation error at this line likely means your copy of the
// proto package needs to be updated.
const _ = proto.ProtoPackageIsVersion2 // please upgrade the proto package

// TODO: explain different types
type DomainServiceRequestRequestType int32

const (
	DomainServiceRequest_DOMAIN_CERT_REQUEST DomainServiceRequestRequestType = 1
	DomainServiceRequest_MANAGE_POLICY       DomainServiceRequestRequestType = 2
	DomainServiceRequest_REVOKE_CERTIFICATE  DomainServiceRequestRequestType = 3
	DomainServiceRequest_GET_CRL             DomainServiceRequestRequestType = 4
)

var DomainServiceRequestRequestType_name = map[int32]string{
	1: "DOMAIN_CERT_REQUEST",
	2: "MANAGE_POLICY",
	3: "REVOKE_CERTIFICATE",
	4: "GET_CRL",
}
var DomainServiceRequestRequestType_value = map[string]int32{
	"DOMAIN_CERT_REQUEST": 1,
	"MANAGE_POLICY":       2,
	"REVOKE_CERTIFICATE":  3,
	"GET_CRL":             4,
}

func (x DomainServiceRequestRequestType) Enum() *DomainServiceRequestRequestType {
	p := new(DomainServiceRequestRequestType)
	*p = x
	return p
}
func (x DomainServiceRequestRequestType) String() string {
	return proto.EnumName(DomainServiceRequestRequestType_name, int32(x))
}
func (x *DomainServiceRequestRequestType) UnmarshalJSON(data []byte) error {
	value, err := proto.UnmarshalJSONEnum(DomainServiceRequestRequestType_value, data, "DomainServiceRequestRequestType")
	if err != nil {
		return err
	}
	*x = DomainServiceRequestRequestType(value)
	return nil
}
func (DomainServiceRequestRequestType) EnumDescriptor() ([]byte, []int) {
	return fileDescriptor0, []int{0, 0}
}

type DomainServiceRequest struct {
	Type *DomainServiceRequestRequestType `protobuf:"varint,1,opt,name=type,enum=domain_service.DomainServiceRequestRequestType" json:"type,omitempty"`
	// Fields for type: DOMAIN_CERT_REQUEST.
	SerializedHostAttestation []byte `protobuf:"bytes,2,opt,name=serialized_host_attestation" json:"serialized_host_attestation,omitempty"`
	// The program key, serialized in the format that
	// auth.NewKeyPrin() accepts.
	ProgramKey []byte `protobuf:"bytes,3,opt,name=program_key" json:"program_key,omitempty"`
	// Fields for type: REVOKE_CERTIFICATE.
	// This is an attestation signed by the policy key with the statement:
	// policyKey says revoke certificateSerialNumber
	SerializedPolicyAttestation []byte `protobuf:"bytes,4,opt,name=serialized_policy_attestation" json:"serialized_policy_attestation,omitempty"`
	XXX_unrecognized            []byte `json:"-"`
}

func (m *DomainServiceRequest) Reset()                    { *m = DomainServiceRequest{} }
func (m *DomainServiceRequest) String() string            { return proto.CompactTextString(m) }
func (*DomainServiceRequest) ProtoMessage()               {}
func (*DomainServiceRequest) Descriptor() ([]byte, []int) { return fileDescriptor0, []int{0} }

func (m *DomainServiceRequest) GetType() DomainServiceRequestRequestType {
	if m != nil && m.Type != nil {
		return *m.Type
	}
	return DomainServiceRequest_DOMAIN_CERT_REQUEST
}

func (m *DomainServiceRequest) GetSerializedHostAttestation() []byte {
	if m != nil {
		return m.SerializedHostAttestation
	}
	return nil
}

func (m *DomainServiceRequest) GetProgramKey() []byte {
	if m != nil {
		return m.ProgramKey
	}
	return nil
}

func (m *DomainServiceRequest) GetSerializedPolicyAttestation() []byte {
	if m != nil {
		return m.SerializedPolicyAttestation
	}
	return nil
}

type DomainServiceResponse struct {
	ErrorMessage *string `protobuf:"bytes,1,opt,name=error_message" json:"error_message,omitempty"`
	// Fields for response to DOMAIN_CERT_REQUEST.
	DerProgramCert []byte `protobuf:"bytes,2,opt,name=der_program_cert" json:"der_program_cert,omitempty"`
	// Fields for response to GET_CRL.
	Crl              []byte `protobuf:"bytes,3,opt,name=crl" json:"crl,omitempty"`
	XXX_unrecognized []byte `json:"-"`
}

func (m *DomainServiceResponse) Reset()                    { *m = DomainServiceResponse{} }
func (m *DomainServiceResponse) String() string            { return proto.CompactTextString(m) }
func (*DomainServiceResponse) ProtoMessage()               {}
func (*DomainServiceResponse) Descriptor() ([]byte, []int) { return fileDescriptor0, []int{1} }

func (m *DomainServiceResponse) GetErrorMessage() string {
	if m != nil && m.ErrorMessage != nil {
		return *m.ErrorMessage
	}
	return ""
}

func (m *DomainServiceResponse) GetDerProgramCert() []byte {
	if m != nil {
		return m.DerProgramCert
	}
	return nil
}

func (m *DomainServiceResponse) GetCrl() []byte {
	if m != nil {
		return m.Crl
	}
	return nil
}

type TrustedEntities struct {
	TrustedProgramTaoNames []string `protobuf:"bytes,1,rep,name=trusted_program_tao_names" json:"trusted_program_tao_names,omitempty"`
	TrustedHostTaoNames    []string `protobuf:"bytes,2,rep,name=trusted_host_tao_names" json:"trusted_host_tao_names,omitempty"`
	TrustedMachineInfos    []string `protobuf:"bytes,3,rep,name=trusted_machine_infos" json:"trusted_machine_infos,omitempty"`
	TrustedRootCerts       [][]byte `protobuf:"bytes,4,rep,name=trusted_root_certs" json:"trusted_root_certs,omitempty"`
	XXX_unrecognized       []byte   `json:"-"`
}

func (m *TrustedEntities) Reset()                    { *m = TrustedEntities{} }
func (m *TrustedEntities) String() string            { return proto.CompactTextString(m) }
func (*TrustedEntities) ProtoMessage()               {}
func (*TrustedEntities) Descriptor() ([]byte, []int) { return fileDescriptor0, []int{2} }

func (m *TrustedEntities) GetTrustedProgramTaoNames() []string {
	if m != nil {
		return m.TrustedProgramTaoNames
	}
	return nil
}

func (m *TrustedEntities) GetTrustedHostTaoNames() []string {
	if m != nil {
		return m.TrustedHostTaoNames
	}
	return nil
}

func (m *TrustedEntities) GetTrustedMachineInfos() []string {
	if m != nil {
		return m.TrustedMachineInfos
	}
	return nil
}

func (m *TrustedEntities) GetTrustedRootCerts() [][]byte {
	if m != nil {
		return m.TrustedRootCerts
	}
	return nil
}

func init() {
	proto.RegisterType((*DomainServiceRequest)(nil), "domain_service.domain_service_request")
	proto.RegisterType((*DomainServiceResponse)(nil), "domain_service.domain_service_response")
	proto.RegisterType((*TrustedEntities)(nil), "domain_service.trusted_entities")
	proto.RegisterEnum("domain_service.DomainServiceRequestRequestType", DomainServiceRequestRequestType_name, DomainServiceRequestRequestType_value)
}

func init() { proto.RegisterFile("service.proto", fileDescriptor0) }

var fileDescriptor0 = []byte{
	// 369 bytes of a gzipped FileDescriptorProto
	0x1f, 0x8b, 0x08, 0x00, 0x00, 0x09, 0x6e, 0x88, 0x02, 0xff, 0x6c, 0x90, 0xdf, 0x6a, 0xdb, 0x30,
	0x14, 0xc6, 0xb1, 0x1d, 0x18, 0x3b, 0xf9, 0x83, 0xa7, 0x90, 0xc4, 0xdb, 0xc8, 0xd8, 0x3c, 0x06,
	0xb9, 0xf2, 0xc5, 0xf6, 0x04, 0xc6, 0xd3, 0x82, 0x59, 0xfe, 0x6c, 0xae, 0xdb, 0xd2, 0x2b, 0x21,
	0x1c, 0x35, 0x11, 0x8d, 0x2d, 0x57, 0x52, 0x0a, 0xe9, 0x53, 0xf4, 0x05, 0xfa, 0xae, 0x55, 0x9c,
	0x98, 0xd6, 0xb4, 0x57, 0x82, 0xef, 0xf7, 0x7d, 0xe7, 0xe8, 0x3b, 0xd0, 0x55, 0x4c, 0xde, 0xf1,
	0x8c, 0x05, 0xa5, 0x14, 0x5a, 0xa0, 0xde, 0x4a, 0xe4, 0x94, 0x17, 0xe4, 0xa4, 0xfa, 0x8f, 0x36,
	0x0c, 0x9b, 0x12, 0x91, 0xec, 0x76, 0xc7, 0x94, 0x46, 0x21, 0xb4, 0xf4, 0xbe, 0x64, 0x9e, 0xf5,
	0xd5, 0x9a, 0xf4, 0x7e, 0xfe, 0x0a, 0x9a, 0xb6, 0xe0, 0xed, 0x54, 0x70, 0x7a, 0xc9, 0x21, 0x8a,
	0xbe, 0xc3, 0x67, 0xc3, 0x39, 0xdd, 0xf2, 0x7b, 0xb6, 0x22, 0x1b, 0x61, 0x74, 0xaa, 0xb5, 0xc1,
	0x54, 0x73, 0x51, 0x78, 0xb6, 0x99, 0xdc, 0x41, 0x7d, 0x68, 0x9b, 0xbf, 0xad, 0x25, 0xcd, 0xc9,
	0x0d, 0xdb, 0x7b, 0x4e, 0x25, 0xfe, 0x80, 0xf1, 0x8b, 0x64, 0x29, 0xb6, 0x3c, 0xdb, 0x37, 0xb2,
	0xad, 0x83, 0xcd, 0x27, 0xd0, 0x69, 0x2c, 0x1c, 0x41, 0xff, 0xf7, 0x72, 0x1e, 0xc6, 0x0b, 0x12,
	0xe1, 0x24, 0x25, 0x09, 0xfe, 0x7f, 0x8e, 0xcf, 0x52, 0xd7, 0x42, 0x1f, 0xa0, 0x3b, 0x0f, 0x17,
	0xe1, 0x14, 0x93, 0x7f, 0xcb, 0x59, 0x1c, 0x5d, 0xb9, 0x36, 0x1a, 0x02, 0x4a, 0xf0, 0xc5, 0xf2,
	0x2f, 0xae, 0xbc, 0xf1, 0x9f, 0x38, 0x0a, 0x53, 0xec, 0x3a, 0xa8, 0x0d, 0xef, 0xa6, 0x38, 0x25,
	0x51, 0x32, 0x73, 0x5b, 0xfe, 0x25, 0x8c, 0x5e, 0x15, 0x55, 0xa5, 0x28, 0x14, 0x43, 0x03, 0xe8,
	0x32, 0x29, 0x85, 0x24, 0x39, 0x53, 0x8a, 0xae, 0x8f, 0x87, 0x7a, 0x8f, 0x3c, 0x70, 0x57, 0x4c,
	0x92, 0xba, 0x52, 0xc6, 0xa4, 0x3e, 0x15, 0x6d, 0x83, 0x93, 0xc9, 0xed, 0xb1, 0xa0, 0xff, 0x60,
	0x81, 0xab, 0xe5, 0x4e, 0x69, 0x53, 0x8f, 0x15, 0x9a, 0x6b, 0xce, 0x14, 0xfa, 0x06, 0x1f, 0x6b,
	0xad, 0xce, 0x6b, 0x2a, 0x48, 0x41, 0xcd, 0x0a, 0x33, 0xde, 0x31, 0xe3, 0xbf, 0xc0, 0xb0, 0xb6,
	0x54, 0xf7, 0x7c, 0xe6, 0x76, 0xc5, 0xc7, 0x30, 0xa8, 0x79, 0x4e, 0xb3, 0x0d, 0x2f, 0x18, 0xe1,
	0xc5, 0xb5, 0x50, 0x66, 0xed, 0x01, 0x7f, 0x02, 0x54, 0x63, 0x29, 0x84, 0xae, 0xbe, 0xa7, 0xcc,
	0x31, 0x9d, 0x49, 0xe7, 0x29, 0x00, 0x00, 0xff, 0xff, 0x16, 0x20, 0x58, 0x89, 0x2b, 0x02, 0x00,
	0x00,
}
