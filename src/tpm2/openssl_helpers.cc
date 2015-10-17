#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <tpm20.h>
#include <tpm2_lib.h>
#include <errno.h>

#include <tpm2.pb.h>
#include <openssl_helpers.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <openssl/asn1.h>

#include <string>
using std::string;

//
// Copyright 2015 Google Corporation, All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//     http://www.apache.org/licenses/LICENSE-2.0
// or in the the file LICENSE-2.0.txt in the top level sourcedirectory
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License
//
// Portions of this code were derived TPM2.0-TSS published
// by Intel under the license set forth in intel_license.txt
// and downloaded on or about August 6, 2015.
// File: openssl_helpers.cc

// standard buffer size
#define MAX_SIZE_PARAMS 4096

void print_cert_request_message(x509_cert_request_parameters_message& req_message) {
  if (req_message.has_common_name()) {
    printf("common name: %s\n", req_message.common_name().c_str());
  }
  if (req_message.has_country_name()) {
    printf("country name: %s\n", req_message.country_name().c_str());
  }
  if (req_message.has_state_name()) {
    printf("state name: %s\n", req_message.state_name().c_str());
  }
  if (req_message.has_locality_name()) {
    printf("locality name: %s\n", req_message.locality_name().c_str());
  }
  if (req_message.has_organization_name()) {
    printf("organization name: %s\n", req_message.organization_name().c_str());
  }
  if (req_message.has_suborganization_name()) {
    printf("suborganization name: %s\n", req_message.suborganization_name().c_str());
  }
  if (!req_message.has_key())
    return;
  if (req_message.key().has_key_type()) {
    printf("key_type name: %s\n", req_message.key().key_type().c_str());
  }
  if (req_message.key().rsa_key().has_key_name()) {
    printf("key name: %s\n", req_message.key().rsa_key().key_name().c_str());
  }
  if (req_message.key().rsa_key().has_bit_modulus_size()) {
    printf("modulus bit size: %d\n", req_message.key().rsa_key().bit_modulus_size());
  }
  if (req_message.key().rsa_key().has_exponent()) {
    string exp = req_message.key().rsa_key().exponent();
    printf("exponent: ");
    PrintBytes(exp.size(), (byte*)exp.data());
    printf("\n");
  }
  if (req_message.key().rsa_key().has_modulus()) {
    string mod = req_message.key().rsa_key().modulus();
    printf("modulus : ");
    PrintBytes(mod.size(), (byte*)mod.data());
    printf("\n");
  }
}

void print_internal_private_key(RSA& key) {
  printf("\n\n");
  printf("\nModulus: \n");
  BN_print_fp(stdout, key.n);
  printf("\n\n");
  printf("\ne: \n");
  BN_print_fp(stdout, key.e);
  printf("\n\n");
  printf("\nd: \n");
  BN_print_fp(stdout, key.d);
  printf("\n\n");
  printf("\np: \n");
  BN_print_fp(stdout, key.p);
  printf("\n\n");
  printf("\nq: \n");
  BN_print_fp(stdout, key.q);
  printf("\n\n");
  printf("\ndmp1: \n");
  BN_print_fp(stdout, key.dmp1);
  printf("\n\n");
  printf("\ndmq1: \n");
  BN_print_fp(stdout, key.dmq1);
  printf("\n\n");
  printf("\niqmp: \n");
  BN_print_fp(stdout, key.iqmp);
  printf("\n\n");
}

BIGNUM* bin_to_BN(int len, byte* buf) {
  BIGNUM* bn = BN_bin2bn(buf, len, nullptr);
  return bn;
}


string* BN_to_bin(BIGNUM& n) {
  byte buf[MAX_SIZE_PARAMS];

  int len = BN_bn2bin(&n, buf);
  return new string((const char*)buf, len);
}

bool GenerateX509CertificateRequest(x509_cert_request_parameters_message&
        params, bool sign_request, X509_REQ* req) {
  RSA*  rsa = nullptr;
  X509_NAME* subject = X509_NAME_new();
  EVP_PKEY* pKey = new EVP_PKEY();

  if (params.key().key_type() != "RSA") {
    printf("Only rsa keys supported %s\n", params.key().key_type().c_str());
    return false;
  }
  if (subject == nullptr) {
    printf("Can't alloc x509 name\n");
    return false;
  }
  if (params.has_common_name()) {
    int nid = OBJ_txt2nid("commonName");
    X509_NAME_ENTRY* ent = X509_NAME_ENTRY_create_by_NID(nullptr, nid,
        MBSTRING_ASC, (byte*)params.common_name().c_str(), -1);
    if (X509_NAME_add_entry(subject, ent, -1, 0) != 1) {
      printf("Can't add name ent\n");
      return false;
    }
  }
  // TODO: do the foregoing for the other name components
  if (X509_REQ_set_subject_name(req, subject) != 1)  {
    printf("Can't set x509 subject\n");
    return false;
  }
  if (!GetPublicRsaKeyFromParameters(params.key().rsa_key(), rsa)) {
    printf("Can't make rsa key\n");
    return false;
  }
  // fill key parameters in request

  if (sign_request) {
#if 0
  EVP_PKEY* pkey = EVP_PKEY_new();
  EVP_MD* digest;
  pkey->type = EVP_PKEY_RSA;
  // check type
  digest = EVP_sha256();
  EVP_PKEY_set1_RSA(pKey, rsa);
  X509_REQ_set_pubkey(req, pkey);
  if (!X509_REQ_sign(req, pkey, digest)) {
    printf("Sign request fails\n");
    return false;
  }
#else
  printf("Signed certificate requests not supported yet\n");
  return false;
#endif
  }
  print_cert_request_message(params);
  return true;
}

bool GetPrivateRsaKeyFromParameters(const rsa_public_key_message& key_msg,
                                    RSA* rsa) {
  return true;
}

bool GetPublicRsaKeyFromParameters(const rsa_public_key_message& key_msg,
                                   RSA* rsa) {
  return true;
}

bool GetPublicRsaParametersFromSSLKey(RSA& rsa, public_key_message* key_msg) {
  string* n = nullptr;
  string* e = nullptr;
  bool ret = true;

#if 0
  n = BN_to_bin(key_msg->n);
  if (n == nullptr) {
    ret = false;
    goto done;
  }
  e = BN_to_bin(key_msg->e);
  if (e == nullptr) {
    ret = false;
    goto done;
  }
  if (key_msg->public_key().key_type() != "RSA") {
  }
  key_msg->mutable_public_key()->mutable_rsa_key()->set_modulus(*n);
  key_msg->mutable_public_key()->mutable_rsa_key()->set_exponent(*e);
#endif

done:
  if (e != nullptr)
    delete e;
  if (n != nullptr)
    delete n;
  return ret;
}

bool GetPrivateRsaParametersFromSSLKey(RSA& rsa, rsa_private_key_message* key_msg) {
  string* d = nullptr;
  string* p = nullptr;
  string* q = nullptr;
  bool ret = true;

#if 0
  if (!GetPublicRsaParametersFromSSLKey(rsa, key_msg->key())) {
    ret = false;
    goto done;
  }
  d = BN_to_bin(key_msg->d);
  if (d == nullptr) {
    ret = false;
    goto done;
  }
  p = BN_to_bin(key_msg->p);
  if (p == nullptr) {
    ret = false;
    goto done;
  }
  q = BN_to_bin(key_msg->q);
  if (q == nullptr) {
    ret = false;
    goto done;
  }
#endif

done:
  return ret;
}

struct entry {
  char* key;
  char* value;
};

#define EXT_COUNT 5
struct entry ext_ent[EXT_COUNT] = {
  {"basicConstraints", "CA:FALSE"},
  {"nsComment", "OpenSSL Cert"},
  {"subjectKeyIdentifier", "hash"},
  {"authorityKeyIdentifier", "keyid, issuer:always"},
  {"keyUsage", "nonrepudiation,digitalSignature,keyEncipherment"},
};

bool SignX509Certificate(RSA* signing_key,
                         signing_instructions_message& signing_instructions,
                         X509_REQ* req, bool verify_req_sig, X509* cert) {
  EVP_PKEY* pSignedKey = X509_REQ_get_pubkey(req);
  if (pSignedKey != nullptr) {
    printf("Can't get pubkey\n");
    return false;
  }

  if (verify_req_sig) {
#if 0
    if (X509_REQ_verify(req, pSignedKey) != 1) {
      printf("Req does not verify\n");
      return false;
    }
#else
    printf("Signed certificate requests not accepted\n");
    return false;
#endif
  }
  
  uint64_t serial = 1;
  EVP_PKEY* pSigningKey= EVP_PKEY_new();
  const EVP_MD* digest = EVP_sha256();
  X509_NAME* name;
  EVP_PKEY_set1_RSA(pSigningKey, signing_key);
  pSigningKey->type = EVP_PKEY_RSA;
  
  X509_set_version(cert, 2L);
  ASN1_INTEGER_set(X509_get_serialNumber(cert), serial++);

  name = X509_REQ_get_subject_name(req);
  if (X509_set_subject_name(cert, name) != 1) {
    printf("Can't set subject name\n");
    return false;
  }
  if (X509_set_pubkey(cert, pSigningKey) != 1) {
    printf("Can't set pubkey\n");
    return false;
  }
  if (!X509_gmtime_adj(X509_get_notBefore(cert), 0)) {
    printf("Can't adj notBefore\n");
    return false;
  }
  if (!X509_gmtime_adj(X509_get_notAfter(cert),
                       signing_instructions.duration())) {
    printf("Can't adj notAfter\n");
    return false;
  }
  X509_NAME* issuer = X509_NAME_new();
  int nid = OBJ_txt2nid("issuerName");
  X509_NAME_ENTRY* ent = X509_NAME_ENTRY_create_by_NID(nullptr, nid,
      MBSTRING_ASC, (byte*)signing_instructions.issuer().c_str(), -1);
  if (X509_NAME_add_entry(issuer, ent, -1, 0) != 1) {
    printf("Can't add issuer name ent\n");
    return false;
  }
  if (X509_set_issuer_name(cert, issuer) != 1) {
    printf("Can't set issuer name\n");
    return false;
  }
#if 0
  // add extensions
  X509V3_CTX ctx;
  X509V3_set_ctx(&ctx, NULL, NULL, NULL, NULL, 0);
  for (int i = 0; i < EXT_COUNT; i++) {
    X509_EXTENSION* ext = X509V3_EXT_conf(nullptr, &ctx,
        ext_ent[i].key, ext_ent[i].value);
    if (ext == 0) {
      printf("Bad ext_conf\n");
      return false;
    }
    if (!X509_add_ext(cert, ext, -1)) {
      printf("Bad add ext %d\n", i);
      return false;
    }
    X509_EXTENSION_free(ext);
  }
#endif
printf("about to call sign\n");
  if (!X509_sign(cert, pSigningKey, digest)) {
    printf("Bad PKEY type\n");
    return false;
  }

  printf("digest->size: %d\n", digest->md_size);
  PrintBytes(digest->md_size, (byte*)digest->final);
  printf("\n");
  return true;
}

bool VerifyX509CertificateChain(certificate_chain_message& chain) {
  // first cert is self signed root
  return false;
}

bool GetCertificateRequestParametersFromX509(X509_REQ& x509_req, cert_parameters* cert_params) {
  return false;
}

bool GetCertificateParametersFromX509(X509& x509_cert, cert_parameters* cert_params) {
  return false;
}

