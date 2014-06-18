// Code generated by protoc-gen-go.
// source: attestation.proto
// DO NOT EDIT!

package tao

import proto "code.google.com/p/goprotobuf/proto"
import math "math"

// Reference imports to suppress errors if they are not otherwise used.
var _ = proto.Marshal
var _ = math.Inf

// A Statement conveys:
//   issuer says ((time <= now < exp) implies ...)
// where "..." is some message conveyed by the remaining fields in the
// statement. By itself, a Statement can't be verified as it does not include a
// signature.
//
// Note: The 'now' predicate here is not properly scoped. For now, whoever is
// using or validating a statement, e.g. a guard, fills in their own value of
// 'now'.
//
// Eventually, a statement might convey a variety of information. Currently,
// only two interpretations are supported.
//
// Delegation Statements:
//   issuer says (... implies (delegate speaksfor issuer))
// Here, delegate is the name of some principal, usually a (non-tpm) Tao key
// principal, i.e. Key("...") where "..." is a base64w-encoded, serialized
// public key as produced by Keys::GetPrincipalName() or
// VerifierToPrincipalName(). If time restrictions are met, from a delegation
// statement we can derive:
//   issuer says (delegate speaksfor issuer)
// and further:
//   delegate speaksfor issuer
// That is to say, a delegation statement conveys a speaks-for relationship.
//
// Delegation Statement Example:
// Suppose some program Prog(...), running on some OS PCRs(...), running on some
// TPM K_aik, generates a key and wants to arrange for that key to
// speak for itself. It can express as much with this statement:
//  Statement {
//    issuer = K_aik::PCRs(...)::Prog(...)
//    time = today
//    expiration = next year
//    delegate = K_prog
//  }
//
// Predicate Statements:
//   issuer says (... implies pred(arg_1, ..., arg_n))
// Here, 'pred' is the name of some predicate that is meaningful to the issuer
// (and to other principals that trust the issuer), and the arg_i are values
// such as strings, integers, or names of principals or subprincipals. If time
// restrictions are met, from a predicate statement we can derive:
//   issuer says pred(arg_1, ..., arg_n).
//
// Predicate Statement Examples:
// Following the above example, the policy key might want to express that the
// TPM identified by K_aik is a trusted platform, and the OS identified by
// PCRs(...) is a trusted OS. It can do so with these statements:
//  Statement {
//    issuer = K_policy
//    ...
//    predicate_name = isTrustedPlatform
//    predicate_args = [ K_aik ]
//  }
//  Statement {
//    issuer = K_policy
//    ...
//    predicate_name = isTrustedOS
//    predicate_args = [ PCRs(...) ]
//  }
type Statement struct {
	// The identity of the issuer.
	Issuer *string `protobuf:"bytes,1,req,name=issuer" json:"issuer,omitempty"`
	// The earliest time this statement is to be used, in 64-bit POSIX time.
	// Typically, this is simply the time at which the statement was created.
	Time *int64 `protobuf:"varint,2,req,name=time" json:"time,omitempty"`
	// The latest time this statement is to be used, in 64-bit POSIX time.
	Expiration *int64 `protobuf:"varint,3,req,name=expiration" json:"expiration,omitempty"`
	// For a delegation credential, the identity of the delegate principal.
	Delegate *string `protobuf:"bytes,4,opt,name=delegate" json:"delegate,omitempty"`
	// For a predicate credential, the name of the predicate and its arguments.
	PredicateName    *string  `protobuf:"bytes,5,opt,name=predicate_name" json:"predicate_name,omitempty"`
	PredicateArgs    []string `protobuf:"bytes,6,rep,name=predicate_args" json:"predicate_args,omitempty"`
	XXX_unrecognized []byte   `json:"-"`
}

func (m *Statement) Reset()         { *m = Statement{} }
func (m *Statement) String() string { return proto.CompactTextString(m) }
func (*Statement) ProtoMessage()    {}

func (m *Statement) GetIssuer() string {
	if m != nil && m.Issuer != nil {
		return *m.Issuer
	}
	return ""
}

func (m *Statement) GetTime() int64 {
	if m != nil && m.Time != nil {
		return *m.Time
	}
	return 0
}

func (m *Statement) GetExpiration() int64 {
	if m != nil && m.Expiration != nil {
		return *m.Expiration
	}
	return 0
}

func (m *Statement) GetDelegate() string {
	if m != nil && m.Delegate != nil {
		return *m.Delegate
	}
	return ""
}

func (m *Statement) GetPredicateName() string {
	if m != nil && m.PredicateName != nil {
		return *m.PredicateName
	}
	return ""
}

func (m *Statement) GetPredicateArgs() []string {
	if m != nil {
		return m.PredicateArgs
	}
	return nil
}

// An Attestation conveys:
//   signer says statement
// i.e.
//   signer says (issuer says ((time <= now < exp) implies ...))
// A valid Attestation encodes a public key within the signer name, and it
// carries a signature that anyone can verify to (eventually) conclude:
//   issuer says ((time' <= now < exp') implies ...)
// Note: Because of time restrictions within attached delegations, the time
// restriction (time' <= now < exp') here does not necessarily exactly
// match the restriction (time <= now < exp) on the serialized statement.
// If the modified time restriction is met, then we can derive the same
// conclusion as we would for the included statement, i.e.:
//   delgate speaksfor issuer               (for a delegation statement)
// or:
//   issuer says pred(arg_1, ..., arg_n)    (for a predicate statement)
// That is to say, a valid Attestation that meets its time restriction conveys
// exactly the same meaning as the included statement conveys.
//
// There are two categories of Attestations:
//
// (1) In cases where issuer is a subprincipal of (or identical to) signer, no
// delegation will be present. In these cases, signer speaksfor issuer, so from
// the attestation:
//   signer says (issuer says ...)
// we can derive:
//   issuer says (issuer says ...)
// and further:
//   issuer says ...
//
// Example of a category (1) attestation:
//   Attestation = {
//     statement = {
//       issuer = K_aik::PCRs(...)
//       ...
//       delegate = K_os
//     }
//     signer = K_aik
//     signature = ...
//     delgation = null
//   }
// Here, an OS has published a delegation statement establishing that key K_os
// speaks for the OS, and this statement was signed by the TPM K_aik on behalf
// of the OS. Note that the OS is a subprincipal of the TPM, so the TPM speaks
// for the OS.
//
// (2) In all other cases, a delegation will be present that, if valid, conveys:
//   issuer0 says ((time0 <= now < exp0) implies (delegate speaksfor issuer0))
// where issuer is a subprincipal of (or identical to) issuer0 and delegate is a
// subprincipal of (or identical to) signer. Such a valid
// delegation can be combined with:
//   signer says (
//      issuer says (
//         (time <= now < exp) implies ...))
// to derive:
//   issuer0 says (
//     (time0 <= now < exp0) implies (issuer says (
//        (time <= now < exp) implies ...)))
// And because issuer0 speaks for issuer, we can further derive:
//   issuer says ((time' <= now < exp') implies ...)
// where time' = max(time, time0) and exp = min(exp, exp0).
//
// Example of a category (2) attestation:
//   Attestation = {
//     statement = {
//       issuer = K_aik::PCRs(...)::Prog(...)
//       ...
//       delegate = K_app
//     }
//     signer = K_os
//     signature = ...
//     delgation = {
//       statement = {
//         issuer = K_aik::PCRs(...)
//         ...
//         delegate = K_os
//       }
//       signer = K_aik
//       signature = ...
//       delgation = null
//     }
//   }
// Here, the OS identified by K_aik::PCRs(...) has signed, using a
// seemingly unrelated key K_os, a statement on behalf of one of its hosted
// programs, K_aik::PCRs(...)::Prog(...). The embeded delegation statement,
// signed by K_aik, binds that seemingly unrelated key K_os to the OS's actual
// identity, K_aik::PCRs(...).
//
// Verifiying an attestion signature requires knowing how the signature was
// produced. We currently define two signature schemes:
//
// (a) Some signatures are produced by the TPM, so here we are bound by the
// mechanisms implemented by the TPM. In this case, we encode the signer name as
//   TPM("..K..") where "..K.." is the serialized, base64w-encoded public half
// of the TPM's RSA key K. The TPM only ever signs things on behalf of its
// hosted programs, so the issuer used in the serialized statement will always
// have the form:
//   TPM("..K..")::PCRs("..i..", "..h..")...
// where "..i.." is a sorted, comma-separated list of PCR numbers, and "..h.."
// is the corresponding, comma-separated list of hex-encoded PCR values. The
// signature is computed roughly as:
//   sig = rsa_sign(K, H( H(issuer|time|expiration|...) | pcrbuf(i, h) ))
// Here, we first hash the statement in a tpm-specific way, then signing the
// hash with RSA key K. To obtain the statement hash, first hash the serialized
// statement, including  issuer, time, expiration and other information. This
// intermediate hash is then re-hashed with a tpm-specific encoding of the PCR
// numbers ("..i..") and values ("..h..") extracted from issuer.
//
// Note: The PCR values are effectively hashed twice, once as part of issuer, and
// separately as part of the pcrbuf datastructure. See optimization note below.
//
// (b) Other signatures are produced in software, and here we have flexibility
// to use simpler signature schemes. In this case, we encode the signer name as
//   Key("..K..") where "..K.." is the serialized, base64w-encoded public half
// of a DSA key K. The issuer used in the serialized statement can have any
// form. The signature is roughly:
//   sig = dsa_sign(K, H(context|issuer|time|expiration|...))
// Here, we simply hash the serialized statement, then sign it with the private
// DSA key K. The context is defined in Tao::AttestationSigningContext.
//
// Together, this results in four possible combinations:
//
// (1a) No delegation, Tao signature.
//      This is the old "ROOT" attestation type.
//      Typically (or maybe always?), signer is the policy key.
//      The signer is always Key("..K..").
// (1b) No delegation, TPM signature.
//      This is produced by TPMTaoChildChannel.
//      The signer is always TPM("..K..") and the statement issuer is
//      always a TPM("..K..")::PCRs("..i..", "..h..")...
// (2a) Delegation, Tao signature.
//      This is the old "INTERMEDIATE" attestation type.
//      The signer is always Key("..K..").
//      The delegation is the head of a chain that eventually terminates in a
//      type (1a) or (1b) attestation.
// (2b) Delegation, TPM signature.
//      This is the old "TPM_1_2_QUOTE" attestation type, but is no longer used.
//      The signer would be TPM("..K..") and the statement issuer would be
//      something like K_policy::TrustedPlatform.
//      The delegation would be the head of a chain that eventually terminates
//      in a type (1a) or (1b) attestation. The issuer at the head of the chain
//      would always be TPM("..K..")::PCRs("..i..", "..h..")
type Attestation struct {
	// A serialized Statement. This is serialized to avoid canonicalization issues
	// when signing and verifying signatures.
	SerializedStatement []byte `protobuf:"bytes,1,req,name=serialized_statement" json:"serialized_statement,omitempty"`
	// The signer's public key, encoded as a principal name.
	Signer *string `protobuf:"bytes,2,req,name=signer" json:"signer,omitempty"`
	// Signature over the serialized statement using TPM or Tao signing.
	Signature []byte `protobuf:"bytes,3,req,name=signature" json:"signature,omitempty"`
	// A delegation that conveys (eventually) that signer speaks for the issuer in
	// the serialized statement. If this is empty, then it must be self evident
	// that signer speaks for the issuer in the serialized statement. This can be
	// added, removed, or replaced without changing the attestation signature, but
	// verification may fail if a required delegation is missing.
	SerializedDelegation []byte `protobuf:"bytes,4,opt,name=serialized_delegation" json:"serialized_delegation,omitempty"`
	// An optional set of further attestations that may pertain, in some way, to
	// the the issuer or signer of this attestation. These can be added or removed
	// without changing the attestation signature. This allows attestations to be
	// piggy-backed, e.g. when an authorization guard requires multiple
	// attestations to check a policy.
	SerializedEndorsements [][]byte `protobuf:"bytes,5,rep,name=serialized_endorsements" json:"serialized_endorsements,omitempty"`
	XXX_unrecognized       []byte   `json:"-"`
}

func (m *Attestation) Reset()         { *m = Attestation{} }
func (m *Attestation) String() string { return proto.CompactTextString(m) }
func (*Attestation) ProtoMessage()    {}

func (m *Attestation) GetSerializedStatement() []byte {
	if m != nil {
		return m.SerializedStatement
	}
	return nil
}

func (m *Attestation) GetSigner() string {
	if m != nil && m.Signer != nil {
		return *m.Signer
	}
	return ""
}

func (m *Attestation) GetSignature() []byte {
	if m != nil {
		return m.Signature
	}
	return nil
}

func (m *Attestation) GetSerializedDelegation() []byte {
	if m != nil {
		return m.SerializedDelegation
	}
	return nil
}

func (m *Attestation) GetSerializedEndorsements() [][]byte {
	if m != nil {
		return m.SerializedEndorsements
	}
	return nil
}

func init() {
}
