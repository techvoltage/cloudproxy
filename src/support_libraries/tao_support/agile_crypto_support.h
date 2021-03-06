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

#ifndef __AGILE_CRYPTO_SUPPORT_H__
#define __AGILE_CRYPTO_SUPPORT_H__

#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <list>

#include "taosupport.pb.h"
#include "keys.pb.h"

#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>

#ifndef byte
typedef unsigned char byte;
#endif

using std::string;

extern string Basic128BitCipherSuite;
extern string Basic192BitCipherSuite;
extern string Basic256BitCipherSuite;

class Signer {
public:
  tao::CryptoHeader* ch_;
  EVP_PKEY* sk_;

  bool Sign(string& in, string* out);
  bool Verify(string& msg, string& sig);
};

class Verifier {
public:
  tao::CryptoHeader* ch_;
  EVP_PKEY* vk_;

  bool Verify(string& msg, string& serialized_sig);
};

class Crypter {
public:
  tao::CryptoHeader* ch_;
  string* encryptingKeyBytes_;
  string* hmacKeyBytes_;

  bool Encrypt(string& in, string* iv, string* mac, string* out);
  bool Decrypt(string& in, string& iv, string& mac, string* out);
};

class Deriver {
public:
  tao::CryptoHeader* ch_;
  string* secret_bytes_;

  bool Derive(string& salt, string& context, string& in,  string* out);
};

bool CrypterAlgorithmNameFromCipherSuite(string& cipher_suite, string* crypter_name);
bool SignerAlgorithmNameFromCipherSuite(string& cipher_suite, string* signer_name);

Verifier* CryptoKeyToVerifier(tao::CryptoKey& ck);
Signer* CryptoKeyToSigner(tao::CryptoKey& ck);
Crypter* CryptoKeyToCrypter(tao::CryptoKey& ck);
Verifier* VerifierFromSigner(Signer* s);
Verifier* VerifierFromCertificate(string& der);
bool GenerateCryptoKey(string& type, tao::CryptoKey* ck);
tao::CryptoKey* SignerToCryptoKey(Signer* s);
tao::CryptoKey* VerifierToCryptoKey(Verifier* v);
tao::CryptoKey* CrypterToCryptoKey(Crypter* c);
bool SerializeECCKeyComponents(EC_KEY* ec_key, string* component);
bool DeserializeECCKeyComponents(string component, EC_KEY* ec_key);

bool Protect(Crypter& crypter, string& in, string* out);
bool Unprotect(Crypter& crypter, string& in, string* out);
bool KeyPrincipalBytes(Verifier* v, string* out);
bool UniversalKeyName(Verifier* v, string* out);

void PrintBytes(int size, byte* buf);
bool ReadFile(string& file_name, string* out);
bool WriteFile(string& file_name, string& in);
void PrintCryptoHeader(const tao::CryptoHeader& ch);
void PrintCryptoKey(const tao::CryptoKey& ck);
#endif


