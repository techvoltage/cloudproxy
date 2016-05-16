//  Copyright (c) 2014, Google Inc.  All rights reserved.
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
#include <string>
#include <stdlib.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "tao/fd_message_channel.h"
#include "tao/tao_rpc.h"
#include "tao/util.h"

#include "helpers.h"
#include "taosupport.h"
#include <taosupport.pb.h>

#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/rand.h>

using std::string;
using std::unique_ptr;

using tao::Base64WDecode;
using tao::Base64WEncode;
using tao::FDMessageChannel;
using tao::InitializeApp;
using tao::MarshalSpeaksfor;
using tao::Tao;
using tao::TaoRPC;

void PrintBytes(int n, byte* in) {
  for (int i = 0; i < n; i++) printf("%02x", in[i]);
}

TaoChannel::TaoChannel() {
}

TaoChannel::~TaoChannel() {
}

bool TaoChannel::OpenTaoChannel(TaoProgramData& client_program_data,
                    string& serverAddress, string& port) {

  // Parse policy cert and program cert.
  if (client_program_data.policy_cert_.size() ==0 ) {
    return false;
  }
  if (client_program_data.policyCertificate_ == nullptr) {
    byte* pc = (byte*)client_program_data.policy_cert_.data();
    client_program_data.policyCertificate_ = d2i_X509(nullptr, (const byte**)&pc,
          client_program_data.policy_cert_.size());
    if (client_program_data.policyCertificate_ == nullptr) {
      return false;
    }
  }
  if (client_program_data.program_cert_.size() ==0 ) {
    return false;
  }
  if (client_program_data.programCertificate_ == nullptr) {
    byte* pc = (byte*)client_program_data.program_cert_.data();
    client_program_data.policyCertificate_ = d2i_X509(nullptr, (const byte**)&pc,
          client_program_data.program_cert_.size());
    if (client_program_data.programCertificate_ == nullptr) {
      return false;
    }
  }

  // Open TLS channel with Program cert.
  string network("tcp");
  if (!peer_channel_.InitSslChannel(network, serverAddress, port,
                    client_program_data.policyCertificate_,
                    client_program_data.programCertificate_,
                    client_program_data.program_key_, true)) {
  }

  // Get peer name from organizational unit.

  return true;
}

void TaoChannel::CloseTaoChannel() {
  peer_channel_.Close();
}

#define BUFSIZE 2048
bool TaoChannel::SendRequest(taosupport::SimpleMessage& out) {
  string msg_buf;

  if (!out.SerializeToString(&msg_buf)) {
    return false;
  }
  int k = peer_channel_.Write(msg_buf.size(), (byte*)msg_buf.data());
  return k > 0;
}

bool TaoChannel::GetRequest(taosupport::SimpleMessage* in) {
  byte buf[BUFSIZE];

  int k = peer_channel_.Read(BUFSIZE, buf);
  if (k <= 0) {
    return false;
  }
  string in_msg;
  in_msg.assign((const char*) buf, k);
  if (!in->ParseFromString(in_msg)) {
    return false;
  }
  return true;
}

TaoProgramData::TaoProgramData() {
  initialized_ = false;
  tao_ = nullptr;
  program_key_ = nullptr;
  size_program_sym_key_ = 0;
  program_sym_key_ = nullptr;
  policy_key_ = nullptr;
  programCertificate_ = nullptr;
  policyCertificate_ = nullptr;
}

TaoProgramData::~TaoProgramData() {
  ClearProgramData();
}

void TaoProgramData::ClearProgramData() {
  initialized_ = false;
  tao_name_.clear();
  policy_cert_.clear();

  tao_ = nullptr;

  // TODO: erase key first.
  // Clear private key.
  if (program_key_ != nullptr) {
    RSA_free(program_key_);
  }
  program_key_ = nullptr;

  if (size_program_sym_key_ > 0 && program_sym_key_ != nullptr) {
    memset(program_sym_key_, 0, size_program_sym_key_);
    free(program_sym_key_);
  }

  if (policyCertificate_ != nullptr) {
    // free it.
  }
  policyCertificate_ = nullptr;
  if (programCertificate_ != nullptr) {
    // free it.
  }
  programCertificate_ = nullptr;
}

bool TaoProgramData::ExtendName(string& subprin) {
  return false;
}

// if (unsealed->compare(bytes) != 0) { }

bool TaoProgramData::InitTao(FDMessageChannel* msg, Tao* tao, string& cfg, string& path,
                              string& network, string& address, string& port) {

  // Set tao and msg for later calls.
  msg_ = msg;
  tao_ = tao;

  // Read policy cert.
  string policy_cert_file = path + "policyCert";
  string cert;
  if (ReadFile(policy_cert_file, &cert)) {
    return false;
  }

  byte* pc = (byte*)policy_cert_.data();

  // Parse policy cert.
  X509* parsed_policy_cert = d2i_X509(nullptr, (const byte**)&pc, policy_cert_.size());
  if (parsed_policy_cert == nullptr) {
    return false;
  }
  EVP_PKEY* evp_policy_key = X509_get_pubkey(parsed_policy_cert);
  policy_key_ = EVP_PKEY_get1_RSA(evp_policy_key);
  if (policy_key_ == nullptr) {
    return false;
  }
  int cert_OK = X509_verify(parsed_policy_cert, X509_get_pubkey(parsed_policy_cert));
  if (cert_OK <= 0) {
    return false;
  }

  // Extend principal name, hash of policy cert identifies policy extension.
  string subprin;
  if (!ExtendName(subprin)) {
    return false;
  }

  // Retrieve extended name.
  if (!tao->GetTaoName(&tao_name_)) {
    return false;
  }

  // Get (or initialize) my symmetric keys.
  if (!InitializeSymmetricKeys(path, 32)) {
    return false;
  }

  // Get (or initialize) my program key.
  if (!InitializeProgramKey(path, 2048, network, address, port)) {
    return false;
  }
  return true;
}

void TaoProgramData::Print() {
  if (!initialized_) {
    printf("Program object is NOT initialized\n");
    return;
  }
  printf("Program object is NOT initialized\n");
  printf("Tao name: %s\n", tao_name_.c_str());
  printf("Policy cert: ");PrintBytes(policy_cert_.size(), (byte*)policy_cert_.data());printf("\n");
  printf("Program key: "); printf("TODO"); printf("\n");
  printf("Sym key: ");PrintBytes(size_program_sym_key_, program_sym_key_);printf("\n");
  printf("Program cert: ");PrintBytes(program_cert_.size(), (byte*)program_cert_.data());printf("\n");
  printf("Program path: %s\n", program_file_path_.c_str());
}

void TaoChannel::Print() {
  printf("Server name: %s\n", server_name_.c_str());
}

bool TaoProgramData::Attest(string& to_attest, string* attested) {
  return tao_->Attest(to_attest, attested);
}

bool TaoProgramData::Seal(string& to_seal, string* sealed) {
  // string encodedBytes; if (!Base64WEncode(to_seal, &encodedBytes)) { }
  return tao_->Seal(to_seal, Tao::SealPolicyDefault, sealed);
}

bool TaoProgramData::Unseal(string& sealed, string* unsealed) {
  string policy;
  return tao_->Unseal(sealed, unsealed, &policy);
}

bool TaoProgramData::RequestDomainServiceCert(string& network, string& address,
                              string& port, string& attestation,
                              string& endorsement_cert,
                              string* program_cert) {

  if (policyCertificate_ == nullptr) {
    return false;
  }

  // Construct temporary channel key.
  RSA* tmpChannelKey = RSA_generate_key(2048, 0x010001ULL, nullptr, nullptr);
  if (tmpChannelKey == nullptr) {
    return false;
  }

  // Self signed cert.
  X509* tmpChannelCert = nullptr;
  /*
  X509* cert = nullptr;
  X509_REQ* req = nullptr;
  if (!SignX509Certificate(tmpChannelKey, true, true,
                         "self", "signing", 86400,
                         EVP_PKEY* signedKey,
                         req, cert);
   */

  SslChannel domainChannel;

  if (!domainChannel.InitSslChannel(network, address, port,
        policyCertificate_, tmpChannelCert, tmpChannelKey, false)) {
    return false;
  }

  // Format request and send it.

  // Get response

  // Fill progam cert.
  return true;
}

bool TaoProgramData::InitializeSymmetricKeys(string& path, int keysize) {
  string sealed;
  string unsealed;
  string file_name = path + "sealedsymmetricKey";

  // Read key file.
  if (ReadFile(file_name, &sealed)) {
    if (!Unseal(sealed, &unsealed)) {
      printf("Can't open InitializeSymmetricKeys\n");
      return false;
    }
    size_program_sym_key_ = unsealed.size();
    program_sym_key_ = (byte*)malloc(size_program_sym_key_);
    memcpy(program_sym_key_, (byte*)unsealed.data(), size_program_sym_key_ );
    memset((byte*)unsealed.data(), 0, size_program_sym_key_);
    return true;
  }

  // Create keys, should really be a call to GetRandom.
  program_sym_key_ = (byte*)malloc(keysize);
  if (program_sym_key_ != nullptr) {
    return false;
  }
  size_program_sym_key_ = keysize;
  if (keysize != RAND_bytes(program_sym_key_, keysize)) {
    return false;
  }

  // Seal the key and save it.
  unsealed.assign((const char*)program_sym_key_, size_program_sym_key_);
  string policy = Tao::SealPolicyDefault;
  if (!tao_->Seal(unsealed, policy, &sealed)) {
    unsealed.clear();
    return false;
  }
  if (!WriteFile(file_name, sealed)) {
    return false;
  }
  unsealed.clear();
  return true;
}

bool TaoProgramData::InitializeProgramKey(string& path, int keysize,
        string& network, string& address, string& port) {

  string sealed_key_file_name = path + "sealedsigningKey";
  string signer_cert_file_name = path + "signerCert";
  string policy_cert_file_name = path + "policyCert";
  string sealed_key;
  string unsealed_key;
  string program_cert;


  // Read and parse policy cert.
  if (!ReadFile(policy_cert_file_name, &policy_cert_)) {
    return false;
  }
  byte* pc = (byte*)policy_cert_.data();
  policyCertificate_ = d2i_X509(nullptr, (const byte**)&pc,
        policy_cert_.size());
  if (policyCertificate_ == nullptr) {
    return false;
  }

  if (ReadFile(sealed_key_file_name, &sealed_key) &&
      ReadFile(signer_cert_file_name, &program_cert)) {
    if (!Unseal(sealed_key, &unsealed_key)) {
      printf("Can't open InitializeProgramKey\n");
      return false;
    }
    // Deserialize the key.
    program_key_ = DeserializeRsaPrivateKey(unsealed_key);
    return true;
  }

  // Generate the key;
  RSA* rsa_key = RSA_generate_key(2048, 0x010001ULL, nullptr, nullptr);
  if (rsa_key == nullptr) {
    return false;
  }
  program_key_ = rsa_key;

  // Get the program cert from the domain service.
  // First we need the endorsement cert.
  string endorsement_cert_file_name = path + "endorsementCert";
  string endorse_cert;
  if (ReadFile(endorsement_cert_file_name, &endorse_cert)) {
    return false;
  }

  // Construct a delegation statement.
  // TODO: make serialized key.
  string serialized_key;
  string msf;
  string attestation;
  if (!MarshalSpeaksfor(serialized_key, tao_name_, &msf)) {
    return false;
  }

  // Get an attestation using delegation and program key;
  if (!Attest(msf, &attestation)) {
    return false;
  }

  // Get Program Cert.
  if (!RequestDomainServiceCert(network, address, port, attestation, endorse_cert,
          &program_cert_)) {
    return false;
  }

  // Save the program cert.
  if (WriteFile(signer_cert_file_name, program_cert)) {
    return false;
  }

  // Serialize and save the RSAKey.
  string out_buf;
  if (!SerializeRsaPrivateKey(rsa_key, &out_buf)) {
    return false;
  }
  if (WriteFile(sealed_key_file_name, out_buf)) {
    return false;
  }
  return true;
}

