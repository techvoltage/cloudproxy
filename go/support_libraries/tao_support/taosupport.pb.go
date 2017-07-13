// Code generated by protoc-gen-go.
// source: taosupport.proto
// DO NOT EDIT!

/*
Package tao_support is a generated protocol buffer package.

It is generated from these files:
	taosupport.proto

It has these top-level messages:
	SavedProgramData
*/
package tao_support

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
// const _ = proto.ProtoPackageIsVersion2 // please upgrade the proto package

type SavedProgramData struct {
	FilePath         *string  `protobuf:"bytes,1,opt,name=file_path,json=filePath" json:"file_path,omitempty"`
	PolicyCert       []byte   `protobuf:"bytes,2,opt,name=policy_cert,json=policyCert" json:"policy_cert,omitempty"`
	ProgramName      *string  `protobuf:"bytes,3,opt,name=program_name,json=programName" json:"program_name,omitempty"`
	SigningKeyBlob   []byte   `protobuf:"bytes,4,opt,name=signing_key_blob,json=signingKeyBlob" json:"signing_key_blob,omitempty"`
	CryptingKeyBlob  []byte   `protobuf:"bytes,5,opt,name=crypting_key_blob,json=cryptingKeyBlob" json:"crypting_key_blob,omitempty"`
	SignerCertChain  [][]byte `protobuf:"bytes,6,rep,name=signer_cert_chain,json=signerCertChain" json:"signer_cert_chain,omitempty"`
	CryptoSuite      *string  `protobuf:"bytes,7,opt,name=crypto_suite,json=cryptoSuite" json:"crypto_suite,omitempty"`
	Delegation       []byte   `protobuf:"bytes,8,opt,name=delegation" json:"delegation,omitempty"`
	XXX_unrecognized []byte   `json:"-"`
}

func (m *SavedProgramData) Reset()                    { *m = SavedProgramData{} }
func (m *SavedProgramData) String() string            { return proto.CompactTextString(m) }
func (*SavedProgramData) ProtoMessage()               {}
func (*SavedProgramData) Descriptor() ([]byte, []int) { return fileDescriptor0, []int{0} }

func (m *SavedProgramData) GetFilePath() string {
	if m != nil && m.FilePath != nil {
		return *m.FilePath
	}
	return ""
}

func (m *SavedProgramData) GetPolicyCert() []byte {
	if m != nil {
		return m.PolicyCert
	}
	return nil
}

func (m *SavedProgramData) GetProgramName() string {
	if m != nil && m.ProgramName != nil {
		return *m.ProgramName
	}
	return ""
}

func (m *SavedProgramData) GetSigningKeyBlob() []byte {
	if m != nil {
		return m.SigningKeyBlob
	}
	return nil
}

func (m *SavedProgramData) GetCryptingKeyBlob() []byte {
	if m != nil {
		return m.CryptingKeyBlob
	}
	return nil
}

func (m *SavedProgramData) GetSignerCertChain() [][]byte {
	if m != nil {
		return m.SignerCertChain
	}
	return nil
}

func (m *SavedProgramData) GetCryptoSuite() string {
	if m != nil && m.CryptoSuite != nil {
		return *m.CryptoSuite
	}
	return ""
}

func (m *SavedProgramData) GetDelegation() []byte {
	if m != nil {
		return m.Delegation
	}
	return nil
}

func init() {
	proto.RegisterType((*SavedProgramData)(nil), "tao_support.SavedProgramData")
}

// func init() { proto.RegisterFile("taosupport.proto", fileDescriptor0) }

var fileDescriptor0 = []byte{
	// 247 bytes of a gzipped FileDescriptorProto
	0x1f, 0x8b, 0x08, 0x00, 0x00, 0x09, 0x6e, 0x88, 0x02, 0xff, 0x54, 0xd0, 0xcd, 0x4a, 0x03, 0x31,
	0x10, 0x07, 0x70, 0xda, 0xfa, 0xd1, 0xce, 0x16, 0x5d, 0x73, 0x0a, 0x08, 0x5a, 0x3d, 0x15, 0x0f,
	0x3e, 0x84, 0xf5, 0x26, 0x48, 0x69, 0x1f, 0x20, 0x4c, 0xb7, 0xe3, 0x36, 0xb8, 0x4d, 0x42, 0x76,
	0x14, 0xf2, 0x66, 0x3e, 0x9e, 0xf9, 0x58, 0x51, 0x8f, 0xfb, 0x9b, 0xff, 0x0c, 0xff, 0x0d, 0xd4,
	0x8c, 0xb6, 0xff, 0x70, 0xce, 0x7a, 0x7e, 0x74, 0xde, 0xb2, 0x15, 0x55, 0x14, 0x35, 0xd0, 0xfd,
	0xd7, 0x18, 0xea, 0x2d, 0x7e, 0xd2, 0x7e, 0xed, 0x6d, 0xeb, 0xf1, 0xf8, 0x8c, 0x8c, 0xe2, 0x1a,
	0x66, 0x6f, 0xba, 0x23, 0xe5, 0x90, 0x0f, 0x72, 0xb4, 0x18, 0x2d, 0x67, 0x9b, 0x69, 0x82, 0x75,
	0xfc, 0x16, 0xb7, 0x50, 0x39, 0xdb, 0xe9, 0x26, 0xa8, 0x86, 0x3c, 0xcb, 0x71, 0x1c, 0xcf, 0x37,
	0x50, 0x68, 0x15, 0x45, 0xdc, 0xc1, 0xdc, 0x95, 0x63, 0xca, 0xe0, 0x91, 0xe4, 0x24, 0x1f, 0xa8,
	0x06, 0x7b, 0x8d, 0x24, 0x96, 0x50, 0xf7, 0xba, 0x35, 0xda, 0xb4, 0xea, 0x9d, 0x82, 0xda, 0x75,
	0x76, 0x27, 0x4f, 0xf2, 0xa1, 0x8b, 0xc1, 0x5f, 0x28, 0x3c, 0x45, 0x15, 0x0f, 0x70, 0xd5, 0xf8,
	0xe0, 0xf8, 0x5f, 0xf4, 0x34, 0x47, 0x2f, 0x7f, 0x06, 0x7f, 0xb2, 0x69, 0x9b, 0x7c, 0x6e, 0xa6,
	0x9a, 0x03, 0x6a, 0x23, 0xcf, 0x16, 0x93, 0x94, 0x2d, 0x83, 0xd4, 0x6f, 0x95, 0x38, 0x95, 0xcc,
	0xeb, 0xe9, 0x25, 0x34, 0x93, 0x3c, 0x2f, 0x25, 0x8b, 0x6d, 0x13, 0x89, 0x1b, 0x80, 0x3d, 0x75,
	0xd4, 0x22, 0x6b, 0x6b, 0xe4, 0xb4, 0xfc, 0xe7, 0xaf, 0x7c, 0x07, 0x00, 0x00, 0xff, 0xff, 0x25,
	0x97, 0x3d, 0x90, 0x5a, 0x01, 0x00, 0x00,
}