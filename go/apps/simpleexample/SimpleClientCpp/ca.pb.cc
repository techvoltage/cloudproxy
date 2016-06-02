// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ca.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "ca.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace tao {

namespace {

const ::google::protobuf::Descriptor* CARequest_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  CARequest_reflection_ = NULL;
const ::google::protobuf::Descriptor* CAResponse_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  CAResponse_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* CAType_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_ca_2eproto() {
  protobuf_AddDesc_ca_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "ca.proto");
  GOOGLE_CHECK(file != NULL);
  CARequest_descriptor_ = file->message_type(0);
  static const int CARequest_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CARequest, type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CARequest, attestation_),
  };
  CARequest_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      CARequest_descriptor_,
      CARequest::default_instance_,
      CARequest_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CARequest, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CARequest, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(CARequest));
  CAResponse_descriptor_ = file->message_type(1);
  static const int CAResponse_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CAResponse, type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CAResponse, attestation_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CAResponse, signed_datalog_rules_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CAResponse, signed_acl_set_),
  };
  CAResponse_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      CAResponse_descriptor_,
      CAResponse::default_instance_,
      CAResponse_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CAResponse, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CAResponse, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(CAResponse));
  CAType_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_ca_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    CARequest_descriptor_, &CARequest::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    CAResponse_descriptor_, &CAResponse::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_ca_2eproto() {
  delete CARequest::default_instance_;
  delete CARequest_reflection_;
  delete CAResponse::default_instance_;
  delete CAResponse_reflection_;
}

void protobuf_AddDesc_ca_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::tao::protobuf_AddDesc_attestation_2eproto();
  ::tao::protobuf_AddDesc_datalog_5fguard_2eproto();
  ::tao::protobuf_AddDesc_acl_5fguard_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\010ca.proto\022\003tao\032\021attestation.proto\032\023data"
    "log_guard.proto\032\017acl_guard.proto\"M\n\tCARe"
    "quest\022\031\n\004type\030\001 \002(\0162\013.tao.CAType\022%\n\013atte"
    "station\030\002 \001(\0132\020.tao.Attestation\"\260\001\n\nCARe"
    "sponse\022\031\n\004type\030\001 \002(\0162\013.tao.CAType\022%\n\013att"
    "estation\030\002 \001(\0132\020.tao.Attestation\0225\n\024sign"
    "ed_datalog_rules\030\003 \001(\0132\027.tao.SignedDatal"
    "ogRules\022)\n\016signed_acl_set\030\004 \001(\0132\021.tao.Si"
    "gnedACLSet*W\n\006CAType\022\t\n\005ERROR\020\000\022\017\n\013ATTES"
    "TATION\020\001\022\022\n\016DATALOG_POLICY\020\002\022\016\n\nACL_POLI"
    "CY\020\003\022\r\n\tUNDEFINED\020\004", 419);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "ca.proto", &protobuf_RegisterTypes);
  CARequest::default_instance_ = new CARequest();
  CAResponse::default_instance_ = new CAResponse();
  CARequest::default_instance_->InitAsDefaultInstance();
  CAResponse::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_ca_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_ca_2eproto {
  StaticDescriptorInitializer_ca_2eproto() {
    protobuf_AddDesc_ca_2eproto();
  }
} static_descriptor_initializer_ca_2eproto_;
const ::google::protobuf::EnumDescriptor* CAType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CAType_descriptor_;
}
bool CAType_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}


// ===================================================================

#ifndef _MSC_VER
const int CARequest::kTypeFieldNumber;
const int CARequest::kAttestationFieldNumber;
#endif  // !_MSC_VER

CARequest::CARequest()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void CARequest::InitAsDefaultInstance() {
  attestation_ = const_cast< ::tao::Attestation*>(&::tao::Attestation::default_instance());
}

CARequest::CARequest(const CARequest& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void CARequest::SharedCtor() {
  _cached_size_ = 0;
  type_ = 0;
  attestation_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

CARequest::~CARequest() {
  SharedDtor();
}

void CARequest::SharedDtor() {
  if (this != default_instance_) {
    delete attestation_;
  }
}

void CARequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* CARequest::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CARequest_descriptor_;
}

const CARequest& CARequest::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ca_2eproto();
  return *default_instance_;
}

CARequest* CARequest::default_instance_ = NULL;

CARequest* CARequest::New() const {
  return new CARequest;
}

void CARequest::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    type_ = 0;
    if (has_attestation()) {
      if (attestation_ != NULL) attestation_->::tao::Attestation::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool CARequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .tao.CAType type = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::tao::CAType_IsValid(value)) {
            set_type(static_cast< ::tao::CAType >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_attestation;
        break;
      }

      // optional .tao.Attestation attestation = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_attestation:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_attestation()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void CARequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .tao.CAType type = 1;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->type(), output);
  }

  // optional .tao.Attestation attestation = 2;
  if (has_attestation()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->attestation(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* CARequest::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .tao.CAType type = 1;
  if (has_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->type(), target);
  }

  // optional .tao.Attestation attestation = 2;
  if (has_attestation()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->attestation(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int CARequest::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .tao.CAType type = 1;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->type());
    }

    // optional .tao.Attestation attestation = 2;
    if (has_attestation()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->attestation());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CARequest::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const CARequest* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const CARequest*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void CARequest::MergeFrom(const CARequest& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_type()) {
      set_type(from.type());
    }
    if (from.has_attestation()) {
      mutable_attestation()->::tao::Attestation::MergeFrom(from.attestation());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void CARequest::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CARequest::CopyFrom(const CARequest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CARequest::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  if (has_attestation()) {
    if (!this->attestation().IsInitialized()) return false;
  }
  return true;
}

void CARequest::Swap(CARequest* other) {
  if (other != this) {
    std::swap(type_, other->type_);
    std::swap(attestation_, other->attestation_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata CARequest::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = CARequest_descriptor_;
  metadata.reflection = CARequest_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int CAResponse::kTypeFieldNumber;
const int CAResponse::kAttestationFieldNumber;
const int CAResponse::kSignedDatalogRulesFieldNumber;
const int CAResponse::kSignedAclSetFieldNumber;
#endif  // !_MSC_VER

CAResponse::CAResponse()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void CAResponse::InitAsDefaultInstance() {
  attestation_ = const_cast< ::tao::Attestation*>(&::tao::Attestation::default_instance());
  signed_datalog_rules_ = const_cast< ::tao::SignedDatalogRules*>(&::tao::SignedDatalogRules::default_instance());
  signed_acl_set_ = const_cast< ::tao::SignedACLSet*>(&::tao::SignedACLSet::default_instance());
}

CAResponse::CAResponse(const CAResponse& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void CAResponse::SharedCtor() {
  _cached_size_ = 0;
  type_ = 0;
  attestation_ = NULL;
  signed_datalog_rules_ = NULL;
  signed_acl_set_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

CAResponse::~CAResponse() {
  SharedDtor();
}

void CAResponse::SharedDtor() {
  if (this != default_instance_) {
    delete attestation_;
    delete signed_datalog_rules_;
    delete signed_acl_set_;
  }
}

void CAResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* CAResponse::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CAResponse_descriptor_;
}

const CAResponse& CAResponse::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ca_2eproto();
  return *default_instance_;
}

CAResponse* CAResponse::default_instance_ = NULL;

CAResponse* CAResponse::New() const {
  return new CAResponse;
}

void CAResponse::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    type_ = 0;
    if (has_attestation()) {
      if (attestation_ != NULL) attestation_->::tao::Attestation::Clear();
    }
    if (has_signed_datalog_rules()) {
      if (signed_datalog_rules_ != NULL) signed_datalog_rules_->::tao::SignedDatalogRules::Clear();
    }
    if (has_signed_acl_set()) {
      if (signed_acl_set_ != NULL) signed_acl_set_->::tao::SignedACLSet::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool CAResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .tao.CAType type = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::tao::CAType_IsValid(value)) {
            set_type(static_cast< ::tao::CAType >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_attestation;
        break;
      }

      // optional .tao.Attestation attestation = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_attestation:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_attestation()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(26)) goto parse_signed_datalog_rules;
        break;
      }

      // optional .tao.SignedDatalogRules signed_datalog_rules = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_signed_datalog_rules:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_signed_datalog_rules()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(34)) goto parse_signed_acl_set;
        break;
      }

      // optional .tao.SignedACLSet signed_acl_set = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_signed_acl_set:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_signed_acl_set()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void CAResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required .tao.CAType type = 1;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->type(), output);
  }

  // optional .tao.Attestation attestation = 2;
  if (has_attestation()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, this->attestation(), output);
  }

  // optional .tao.SignedDatalogRules signed_datalog_rules = 3;
  if (has_signed_datalog_rules()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      3, this->signed_datalog_rules(), output);
  }

  // optional .tao.SignedACLSet signed_acl_set = 4;
  if (has_signed_acl_set()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      4, this->signed_acl_set(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* CAResponse::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required .tao.CAType type = 1;
  if (has_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->type(), target);
  }

  // optional .tao.Attestation attestation = 2;
  if (has_attestation()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->attestation(), target);
  }

  // optional .tao.SignedDatalogRules signed_datalog_rules = 3;
  if (has_signed_datalog_rules()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        3, this->signed_datalog_rules(), target);
  }

  // optional .tao.SignedACLSet signed_acl_set = 4;
  if (has_signed_acl_set()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        4, this->signed_acl_set(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int CAResponse::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .tao.CAType type = 1;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->type());
    }

    // optional .tao.Attestation attestation = 2;
    if (has_attestation()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->attestation());
    }

    // optional .tao.SignedDatalogRules signed_datalog_rules = 3;
    if (has_signed_datalog_rules()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->signed_datalog_rules());
    }

    // optional .tao.SignedACLSet signed_acl_set = 4;
    if (has_signed_acl_set()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->signed_acl_set());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CAResponse::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const CAResponse* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const CAResponse*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void CAResponse::MergeFrom(const CAResponse& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_type()) {
      set_type(from.type());
    }
    if (from.has_attestation()) {
      mutable_attestation()->::tao::Attestation::MergeFrom(from.attestation());
    }
    if (from.has_signed_datalog_rules()) {
      mutable_signed_datalog_rules()->::tao::SignedDatalogRules::MergeFrom(from.signed_datalog_rules());
    }
    if (from.has_signed_acl_set()) {
      mutable_signed_acl_set()->::tao::SignedACLSet::MergeFrom(from.signed_acl_set());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void CAResponse::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CAResponse::CopyFrom(const CAResponse& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CAResponse::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  if (has_attestation()) {
    if (!this->attestation().IsInitialized()) return false;
  }
  if (has_signed_datalog_rules()) {
    if (!this->signed_datalog_rules().IsInitialized()) return false;
  }
  if (has_signed_acl_set()) {
    if (!this->signed_acl_set().IsInitialized()) return false;
  }
  return true;
}

void CAResponse::Swap(CAResponse* other) {
  if (other != this) {
    std::swap(type_, other->type_);
    std::swap(attestation_, other->attestation_);
    std::swap(signed_datalog_rules_, other->signed_datalog_rules_);
    std::swap(signed_acl_set_, other->signed_acl_set_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata CAResponse::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = CAResponse_descriptor_;
  metadata.reflection = CAResponse_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace tao

// @@protoc_insertion_point(global_scope)