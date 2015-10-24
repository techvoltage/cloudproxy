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
// File: CreateAndSaveCloudProxyKeyHierarchy.cc


// This program creates a primary key, signing key (for quotes) and sealing key under the owner hierarchy
// and saves them to the cloudproxy defined namespace in nv ram so they can be reloaded.  NV ram
//  is protected with PCR's of current "authenticated boot" so they can only be reread by
//  the same cloudproxy environment.  It optionally seals an input string and quotes a quote string.
//  This program removes existing cloudproxy slots with same names and slot numbers.


// Calling sequence
//   CreateAndSaveCloudProxyKeyHierarchy.exe --cloudproxy_namespace="name"
//      --slot_primary=int32 --slot_seal=int32 --slot_quote=int32
//      --seal_value=value-string --quote_value=value-string --pcr_hash_alg_name=[sha1 | sha256]
//      --pcr_list="int, int, ..." --seal_output_file=output-file-name
//      --quote_output_file= output-file-name --pcr_file=output-file-name

using std::string;


#define CALLING_SEQUENCE "CreateAndSaveCloudProxyKeyHierarchy.exe " \
"--slot_primary=int32 --slot_seal=int32 " \
"--slot_quote=int32 " \
"--seal_value=value-string --quote_value=value-string " \
"--pcr_hash_alg_name=[sha1 | sha256] " \
"--pcr_list='int, int, ...' --seal_output_file=output-file-name " \
"--quote_output_file= output-file-name --pcr_file=output-file-name\n"

void PrintOptions() {
  printf("Calling sequence: %s", CALLING_SEQUENCE);
}

DEFINE_int32(slot_primary, 1, "");
DEFINE_int32(slot_seal, 2, "");
DEFINE_int32(slot_quote, 3, "");
DEFINE_string(seal_value, "", "test seal value");
DEFINE_string(quote_value, "", "test quote value");
DEFINE_string(pcr_hash_alg_name, "", "hash alg (sha1 or sha256");
DEFINE_string(pcr_list, "", "comma separated pcr list");
DEFINE_string(seal_output_file, "", "output-file-name");
DEFINE_string(quote_output_file, "", "output-file-name");
DEFINE_string(pcr_file, "", "output-file-name");

#ifndef GFLAGS_NS
#define GFLAGS_NS gflags
#endif

#define MAX_SIZE_PARAMS 8192

int main(int an, char** av) {
  LocalTpm tpm;
  int ret_val = 0;

  GFLAGS_NS::ParseCommandLineFlags(&an, &av, true);
  if (!tpm.OpenTpm("/dev/tpm0")) {
    printf("Can't open tpm\n");
    return 1;
  }

  // OpenSSL_add_all_algorithms();
  // ERR_load_crypto_strings();

  // Create the Heirarchy 
  string authString("01020304");
  string parentAuth("01020304");
  string emptyAuth;
  
  TPML_PCR_SELECTION pcrSelect;
  TPMA_OBJECT root_flags;

  TPM2B_CREATION_DATA creation_out;
  TPM2B_DIGEST digest_out;
  TPMT_TK_CREATION creation_ticket;

  TPM_HANDLE root_handle = 0; 
  TPM2B_PUBLIC root_pub_out;

  TPM_HANDLE seal_load_handle = 0;
  TPMA_OBJECT seal_create_flags;
  TPM2B_NAME seal_name;
  int seal_size_public = MAX_SIZE_PARAMS;
  byte seal_out_public[MAX_SIZE_PARAMS];
  int seal_size_private = MAX_SIZE_PARAMS;
  byte seal_out_private[MAX_SIZE_PARAMS];

  TPMA_OBJECT quote_create_flags;
  TPM2B_NAME quote_name;
  TPM_HANDLE quote_load_handle = 0;
  int quote_size_public = MAX_SIZE_PARAMS;
  byte quote_out_public[MAX_SIZE_PARAMS];
  int quote_size_private = MAX_SIZE_PARAMS;
  byte quote_out_private[MAX_SIZE_PARAMS];

  TPM_HANDLE nv_handle = 0;
  // uint16_t size_context_save_area =  MAX_SIZE_PARAMS;
  byte context_save_area[MAX_SIZE_PARAMS];
  int context_data_size = MAX_SIZE_PARAMS;

  TPM_HANDLE sealed_load_handle = 0;
  InitSinglePcrSelection(7, TPM_ALG_SHA1, pcrSelect);

  // root of hierarchy 
  *(uint32_t*)(&root_flags) = 0;
  root_flags.fixedTPM = 1;
  root_flags.fixedParent = 1;
  root_flags.sensitiveDataOrigin = 1;
  root_flags.userWithAuth = 1;
  root_flags.decrypt = 1;
  root_flags.restricted = 1;
  if (Tpm2_CreatePrimary(tpm, TPM_RH_OWNER, authString, pcrSelect,
                         TPM_ALG_RSA, TPM_ALG_SHA256, root_flags,
                         TPM_ALG_AES, 128, TPM_ALG_CFB, TPM_ALG_NULL,
                         2048, 0x010001, &root_handle, &root_pub_out)) {
    printf("CreatePrimary root succeeded\n");
  } else {
    printf("CreatePrimary root failed\n");
    ret_val = 1;
    goto done;
  }

  *(uint32_t*)(&seal_create_flags) = 0;
  seal_create_flags.fixedTPM = 1;
  seal_create_flags.fixedParent = 1;
  seal_create_flags.sensitiveDataOrigin = 1;
  seal_create_flags.userWithAuth = 1;
  seal_create_flags.sign = 1;

  if (Tpm2_CreateKey(tpm, root_handle, parentAuth, authString, pcrSelect,
                     TPM_ALG_RSA, TPM_ALG_SHA256, seal_create_flags, TPM_ALG_NULL,
                     (TPMI_AES_KEY_BITS)0, TPM_ALG_ECB, TPM_ALG_RSASSA,
                     2048, 0x010001, &seal_size_public, seal_out_public,
                     &seal_size_private, seal_out_private,
                     &creation_out, &digest_out, &creation_ticket)) {
    printf("Create seal key succeeded private size: %d, public size: %d\n",
           seal_size_private, seal_size_public);
  } else {
    printf("Create seal key failed\n");
    ret_val = 1;
    goto done;
  }

  // load seal key
  if (Tpm2_Load(tpm, root_handle, parentAuth, seal_size_public, seal_out_public,
               seal_size_private, seal_out_private, &seal_load_handle, &seal_name)) {
    printf("Load seal key succeeded\n");
  } else {
    printf("Load seal key failed\n");
    ret_val = 1;
    goto done;
  }

  *(uint32_t*)(&quote_create_flags) = 0;
  quote_create_flags.fixedTPM = 1;
  quote_create_flags.fixedParent = 1;
  quote_create_flags.sensitiveDataOrigin = 1;
  quote_create_flags.userWithAuth = 1;
  quote_create_flags.sign = 1;
  quote_create_flags.restricted = 1;

  if (Tpm2_CreateKey(tpm, root_handle, parentAuth, authString, pcrSelect,
                     TPM_ALG_RSA, TPM_ALG_SHA256, quote_create_flags, TPM_ALG_NULL,
                     (TPMI_AES_KEY_BITS)0, TPM_ALG_ECB, TPM_ALG_RSASSA,
                     1024, 0x010001,
                     &quote_size_public, quote_out_public,
                     &quote_size_private, quote_out_private,
                     &creation_out, &digest_out, &creation_ticket)) {
    printf("Create quote succeeded, private size: %d, public size: %d\n",
           quote_size_private, quote_size_public);
  } else {
    printf("Create quote failed\n");
    ret_val = 1;
    goto done;
  }

  if (Tpm2_Load(tpm, root_handle, parentAuth, quote_size_public, quote_out_public,
               quote_size_private, quote_out_private, &quote_load_handle, &quote_name)) {
    printf("Load quote succeeded\n");
  } else {
    printf("Load quote failed\n");
    ret_val = 1;
    goto done;
  }

  // Save context primary context
  nv_handle = GetNvHandle(FLAGS_slot_primary);
  context_data_size = MAX_SIZE_PARAMS;
  if (!Tpm2_SaveContext(tpm, root_handle, &context_data_size,
                        context_save_area)) {
    printf("Primary SaveContext failed\n");
    ret_val = 1;
    goto done;
  }
  printf("Primary Save context worked, size is %d\n", context_data_size);
  printf("nv_handle: %08x\n", nv_handle);
  printf("Context save area (%d): ", context_data_size);
  PrintBytes(context_data_size, context_save_area);
  printf("\n");
  if (!Tpm2_UndefineSpace(tpm, TPM_RH_OWNER, nv_handle)) {
    printf("Primary UndefinedSpace failed\n");
  }
  // TODO(jlm): should be pcrpolicy protected
  // TODO(jlm): index into structures should be added 
  if (!Tpm2_DefineSpace(tpm, TPM_RH_OWNER, nv_handle,
                        authString, (uint16_t)context_data_size + 32)) {
    printf("Primary DefinedSpace failed\n");
    ret_val = 1;
    goto done;
  }
  printf("Root writing %d bytes\n", context_data_size);
  if (!Tpm2_WriteNv(tpm, nv_handle, authString, 
                    (uint16_t)context_data_size, context_save_area)){
    printf("Primary WriteNv failed\n");
    ret_val = 1;
    goto done;
  }

  // Save seal context
  nv_handle = GetNvHandle(FLAGS_slot_seal);
  context_data_size = MAX_SIZE_PARAMS;
  if (!Tpm2_SaveContext(tpm, seal_load_handle, &context_data_size,
                        context_save_area)) {
    printf("Seal SaveContext failed\n");
    ret_val = 1;
    goto done;
  }
  printf("Seal Save context worked, size is %d\n", context_data_size);
  printf("nv_handle: %08x\n", nv_handle);
  if (!Tpm2_UndefineSpace(tpm, TPM_RH_OWNER, nv_handle)) {
    printf("Seal UndefinedSpace failed\n");
  }
  // TODO(jlm): should be pcrpolicy protected
  // TODO(jlm): index into structures should be added 
  if (!Tpm2_DefineSpace(tpm, TPM_RH_OWNER, nv_handle,
                      authString, (uint16_t)context_data_size + 32)) {
    printf("Seal DefinedSpace failed\n");
    ret_val = 1;
    goto done;
  }
  printf("Seal writing %d bytes\n", context_data_size);
  if (!Tpm2_WriteNv(tpm, nv_handle, authString, 
                    (uint16_t)context_data_size, context_save_area)){
    printf("Seal WriteNv failed\n");
    ret_val = 1;
    goto done;
  }

  // Save quote context
  nv_handle = GetNvHandle(FLAGS_slot_quote);
  context_data_size = MAX_SIZE_PARAMS;
  if (!Tpm2_SaveContext(tpm, quote_load_handle, &context_data_size,
                        context_save_area)) {
    printf("Quote SaveContext failed\n");
    ret_val = 1;
    goto done;
  }
  printf("Quote SaveContext worked, size is %d\n", context_data_size);
  printf("nv_handle: %08x\n", nv_handle);
  if (!Tpm2_UndefineSpace(tpm, TPM_RH_OWNER, nv_handle)) {
    printf("Seal UndefinedSpace failed\n");
  }
  // TODO(jlm): should be pcrpolicy protected
  // TODO(jlm): index into structures should be added 
  if (!Tpm2_DefineSpace(tpm, TPM_RH_OWNER, nv_handle,
                      authString, (uint16_t)context_data_size + 32)) {
    printf("Seal DefinedSpace failed\n");
    ret_val = 1;
    goto done;
  }
  printf("Quote writing %d bytes\n", context_data_size);
  if (!Tpm2_WriteNv(tpm, nv_handle, authString, 
                    (uint16_t)context_data_size, context_save_area)){
    printf("Seal WriteNv failed\n");
    ret_val = 1;
    goto done;
  }

#if 0
  // test seal and quote
  //  seal and quote as test
  //  FLAGS_seal_value
  //  FLAGS_quote_value
  //  FLAGS_seal_output_file
  //  FLAGS_quote_output_file

  if (Tpm2_CreateSealed(tpm, root_handle, policy_digest.size, policy_digest.buffer,
                        parentAuth, secret.size, secret.buffer, pcrSelect,
                        TPM_ALG_SHA1, seal_create_flags, TPM_ALG_NULL,
                        (TPMI_AES_KEY_BITS)0, TPM_ALG_ECB, TPM_ALG_RSASSA,
                        1024, 0x010001,
                        &sealed_size_public, sealed_out_public,
                        &sealed_size_private, sealed_out_private,
                        &creation_out, &digest_out, &creation_ticket)) {
    printf("Create with digest succeeded private size: %d, public size: %d\n",
           sealed_size_private, sealed_size_public);
  } else {
    Tpm2_FlushContext(tpm, root_handle);
    ret_val = 1;
    goto done;
  }

  if (Tpm2_Load(tpm, root_handle, parentAuth, seal_size_public, seal_out_public,
               seal_size_private, seal_out_private, &seal_load_handle, &seal_name)) {
    printf("Load succeeded, handle: %08x\n", seal_load_handle);
  } else {
    ret_val = 1;
    goto done;
  }
#endif

  // FLAGS_pcr_hash_alg_name

  // FLAGS_slot_quote
  // FLAGS_pcr_file
done:
  if (root_handle != 0) {
    Tpm2_FlushContext(tpm, root_handle);
  }
  if (seal_load_handle != 0) {
    Tpm2_FlushContext(tpm, seal_load_handle);
  }
  if (quote_load_handle != 0) {
    Tpm2_FlushContext(tpm, quote_load_handle);
  }
  if (sealed_load_handle != 0) {
    Tpm2_FlushContext(tpm, sealed_load_handle);
  }
  tpm.CloseTpm();
  return ret_val;
}

