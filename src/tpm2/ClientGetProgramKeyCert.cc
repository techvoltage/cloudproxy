#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <tpm20.h>
#include <tpm2_lib.h>
#include <gflags/gflags.h>

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
// Portions of this code were derived tboot published
// by Intel under the license set forth in intel_license.txt
// and downloaded on or about August 6, 2015.
// Portions of this code were derived from the crypto utility
// published by John Manferdelli under the Apache 2.0 license.
// See github.com/jlmucb/crypto.
// File: ClientRetrieveInterimSigningKey.cc


//  This program decrypts the encrypted_signing_key_certificate_file, it then generates and/or
//  retrieves the program  private/public key pair, seals the private portion to
//  the cloudproxy environment and creates a file containing a protobuf with the
//  signing_key_certificate and a protobuf signed by the signing_key
//  naming the public portion of the proposed cloudproxy_program_key.

// Calling sequence: ClientRetrieveInterimSigningKey.exe
//    --cloudproxy_namespace="name"
//    --cloudproxy_slot_primary=slot-number
//    --cloudproxy_slot_seal= slot-number
//    --encrypted_interim_certificate_file=input-file-name
//    --signing_key_namespace=name
//    --signing_key_primary_slot=slot-number
//    --signing_key_signing_slot=slot-number
//    --signing_key_cert_file_=input-file-name
//    --tpm_credential_file=input-file-name
//    --cloudproxy_regenerate_program_key=input-file-name
//    --signed_program_public_key_request_file=output-file-name


using std::string;


#define CALLING_SEQUENCE "ClientRetrieveInterimSigningKey.exe " \
"--cloudproxy_namespace=name " \
"--cloudproxy_slot_primary=slot-number " \
"--cloudproxy_slot_seal= slot-number " \
"--encrypted_certificate_file=input-file-name " \
"--signing_key_primary_slot=slot-number " \
"--signing_key_signing_slot=slot-number " \
"--signing_key_cert_file_=input-file-name " \
"--tpm_credential_file=input-file-name " \
"--program_key_cert_file=output-file-name\n"

void PrintOptions() {
  printf("Calling sequence: %s", CALLING_SEQUENCE);
}


DEFINE_string(encrypted_certificate_file, "", "input-file-name");
DEFINE_int32(primary_slot, 1, "slot-number");
DEFINE_int32(seal_slot, 2, "slot-number");
DEFINE_int32(quote_slot, 3, "slot-number");
DEFINE_string(tpm_credential_file, "", "input-file-name");
DEFINE_string(program_cert_file, "", "output-file-name");

#ifndef GFLAGS_NS
#define GFLAGS_NS gflags
#endif

#define MAX_SIZE_PARAMS 4096

int main(int an, char** av) {
  LocalTpm tpm;
  int ret_val = 0;

  GFLAGS_NS::ParseCommandLineFlags(&an, &av, true);
  if (!tpm.OpenTpm("/dev/tpm0")) {
    printf("Can't open tpm\n");
    return 1;
  }

#if 0
  TPM_HANDLE nv_handle = 0;

  string authString("01020304");
  string parentAuth("01020304");
  string emptyAuth;

  TPML_PCR_SELECTION pcrSelect;

  TPM_HANDLE root_handle = 0;
  TPM_HANDLE seal_handle = 0;
  TPM_HANDLE quote_handle = 0;

  TPM2B_DIGEST credential;
  TPM2B_ID_OBJECT credentialBlob;
  TPM2B_ENCRYPTED_SECRET secret;
  TPM2B_DIGEST recovered_credential;

  // Generate program key
  if (FLAGS_program_key_type != "RSA") {
    printf("Only RSA supported\n");
    ret_val = 1;
    goto done;
  }
  if (FLAGS_program_key_name == "") {
    printf("No key name\n");
    ret_val = 1;
    goto done;
  }
  // read input

  if (!ReadFileIntoBlock(FLAGS_signed_endorsement_cert_file,
                       &ek_cert_blob_size, ek_cert_blob)) {
  printf("Can't read endorsement info\n");
  ret_val = 1;
  goto done;
  }

  // Create endorsement key
  *(uint32_t*)(&primary_flags) = 0;

  primary_flags.fixedTPM = 1;
  primary_flags.fixedParent = 1;
  primary_flags.sensitiveDataOrigin = 1;
  primary_flags.userWithAuth = 1;
  primary_flags.decrypt = 1;
  primary_flags.restricted = 1;

  InitSinglePcrSelection(-1, TPM_ALG_SHA256, pcrSelect);
  if (Tpm2_CreatePrimary(tpm, TPM_RH_ENDORSEMENT, emptyAuth, pcrSelect,
                         TPM_ALG_RSA, TPM_ALG_SHA256, primary_flags,
                         TPM_ALG_AES, 128, TPM_ALG_CFB, TPM_ALG_NULL,
                         2048, 0x010001, &ekHandle, &ek_pub_out)) {
    printf("CreatePrimary succeeded parent: %08x\n", ekHandle);
  } else {
    printf("CreatePrimary failed\n");
    ret_val = 1;
    goto done;
  }
  if (Tpm2_ReadPublic(tpm, ekHandle, &ek_pub_blob_size, ek_pub_blob,
                      ek_pub_out, ek_pub_name, ek_qualified_pub_name)) {
    printf("ek ReadPublic succeeded\n");
  } else {
    printf("ek ReadPublic failed\n");
    ret_val = 1;
    goto done;
  }
  printf("ek Public blob: ");
  PrintBytes(ek_pub_blob_size, ek_pub_blob);
  printf("\n");
  printf("ek Name: ");
  PrintBytes(ek_pub_name.size, ek_pub_name.name);
  printf("\n");
  printf("ek Qualified name: ");
  PrintBytes(ek_qualified_pub_name.size, ek_qualified_pub_name.name);
  printf("\n");

  // restore context
 // TODO(jlm): should get pcr list from parameters
  InitSinglePcrSelection(7, TPM_ALG_SHA1, pcrSelect);

  // root handle
  memset(context_save_area, 0, MAX_SIZE_PARAMS);
  nv_handle = GetNvHandle(FLAGS_primary_slot);
  if (!Tpm2_ReadNv(tpm, nv_handle, authString, (uint16_t) context_data_size,
                   context_save_area)) {
    printf("Root ReadNv failed\n");
    ret_val = 1;
    goto done;
  }
  printf("\ncontext_save_area: ");
  PrintBytes(context_data_size - 6, context_save_area + 6);
  printf("\n\n");
  if (!Tpm2_LoadContext(tpm, context_data_size - 6, context_save_area + 6,
                        &root_handle)) {
    printf("Root LoadContext failed\n");
    ret_val = 1;
    goto done;
  }

  // seal handle
  memset(context_save_area, 0, MAX_SIZE_PARAMS);
  nv_handle = GetNvHandle(FLAGS_seal_slot);
  if (!Tpm2_ReadNv(tpm, nv_handle, authString, (uint16_t)context_data_size,
                   context_save_area)) {
    printf("Root ReadNv failed\n");
    ret_val = 1;
    goto done;
  }
  printf("context_save_area: ");
  PrintBytes(context_data_size, context_save_area);
  printf("\n");
  if (!Tpm2_LoadContext(tpm, context_data_size - 6, context_save_area + 6,
                        &seal_handle)) {
    printf("Root LoadContext failed\n");
    ret_val = 1;
    goto done;


  // quote handle
  memset(context_save_area, 0, MAX_SIZE_PARAMS);
  nv_handle = GetNvHandle(FLAGS_quote_slot);
  if (!Tpm2_ReadNv(tpm, nv_handle, authString, (uint16_t)context_data_size,
                   context_save_area)) {
    printf("Quote ReadNv failed\n");
    ret_val = 1;
    goto done;
  }
  if (!Tpm2_LoadContext(tpm, context_data_size - 6, context_save_area + 6,
                        &quote_handle)) {
    printf("Quote LoadContext failed\n");
    ret_val = 1;
    goto done;
  }

  memset((void*)&credential, 0, sizeof(TPM2B_DIGEST));
  memset((void*)&secret, 0, sizeof(TPM2B_ENCRYPTED_SECRET));
  memset((void*)&credentialBlob, 0, sizeof(TPM2B_ID_OBJECT));
  credential.size = 20;
  for (int i = 0; i < 20; i++)
    credential.buffer[i] = i + 1;

  TPM2B_PUBLIC active_pub_out;
  TPM2B_NAME active_pub_name;
  TPM2B_NAME active_qualified_pub_name;
  uint16_t active_pub_blob_size = 1024;
  byte active_pub_blob[1024];

  memset((void*)&active_pub_out, 0, sizeof(TPM2B_PUBLIC));

  if (Tpm2_ReadPublic(tpm, activeHandle,
                      &active_pub_blob_size, active_pub_blob,
                      active_pub_out, active_pub_name,
                      active_qualified_pub_name)) {
    printf("ReadPublic succeeded\n");
  } else {
    printf("ReadPublic failed\n");
    return false;
  }
  printf("Active Name (%d): ", active_pub_name.size);
  PrintBytes(active_pub_name.size, active_pub_name.name);
  printf("\n");

  printf("credBlob size: %d\n", credentialBlob.size);
  printf("secret size: %d\n", secret.size);
  if (Tpm2_ActivateCredential(tpm, activeHandle, ekHandle,
                              parentAuth, emptyAuth,
                              credentialBlob, secret,
                              &recovered_credential)) {
    printf("ActivateCredential succeeded\n");
    printf("Recovered credential (%d): ", recovered_credential.size);
    PrintBytes(recovered_credential.size, recovered_credential.buffer);
    printf("\n")
  }

 // Write output cert
 request.SerializeToString(&output);
  if (!WriteFileFromBlock(FLAGS_program_cert_request_file,
                          output.size(),
                          (byte*)output.data())) {
    printf("Can't write endorsement cert\n");
    goto done;
  }

  // print out cert
#endif

done:
#if 0
 if (root_handle != 0) {
    Tpm2_FlushContext(tpm, root_handle);
  }
  if (seal_handle != 0) {
    Tpm2_FlushContext(tpm, seal_handle);
  }
  if (quote_handle != 0) {
    Tpm2_FlushContext(tpm, quote_handle);
  }
  if (ekHandle != 0) {
    Tpm2_FlushContext(tpm, ekHandle);
  }
#endif
  tpm.CloseTpm();
  return ret_val;
}
