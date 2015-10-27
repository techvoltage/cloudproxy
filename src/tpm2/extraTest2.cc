
void test_paul(RSA* p_key) {
  int seed_size = 0x14;
  byte seedbuf[] = {
  };
  string seed;
  seed.assign(seed_buf, seed_size);
  int name_size;
  byte name_buf[] = {
  };
  string name;
  name.assign(name_buf, name_size);
  string label("IDENTITY");
  int test_size;
  byte test_buf[4096];
  string contextV;
  int size_hmacKey = 20;

  TPM2B_DIGEST marshaled_credential;
  TPM2B_DIGEST unmarshaled_credential;
  unmarshaled_credential.size = 0x14;

  byte credential_buf[] = {
  };
  memcpy(unmarshaled_credential.buffer, credential_buf, unmarshaled_credential.size);
  ChangeEndian16(&unmarshaled_credential.size, &marshaled_credential.size);
  memcpy(marshaled_credential.buffer, credential_buf, unmarshaled_credential.size);

  printf("symKey should be: \n");
  printf("encId should be: ");
  printf("hmacKey should be: \n");
  printf("outerHmac should be: \n");
  printf("actBlob should be: \n");

  int size_in = 0;
  byte in_buf[4096];
  memcpy(in_buf, seed, size_seed);
  size_in += size_seed;
  memcpy(&in_buf[size_in], (byte*)"IDENTITY", strlen("IDENTITY") + 1);
  size_in += strlen("IDENTITY") + 1;

  // Secret= E(protector_key, seed || "IDENTITY")
  byte encrypted_secret[4096];
  int encrypted_secret_size = RSA_public_encrypt(size_in, in_buf, encrypted_secret,
                                                  p_key, RSA_PKCS1_OAEP_PADDING);

  printf("encrypted_secret_size: %d\n", encrypted_secret_size);
  printf("Encrypted secret: ");
  PrintBytes(encrypted_secret_size, encrypted_secret); printf("\n");
  response.set_secret(encrypted_secret, encrypted_secret_size);
  printf("name: ");
  PrintBytes(name_size, name_buf);
  printf("\n");

  // symKey= KDFa(hash, seed, "STORAGE", name, nullptr, 128);
  string key;
  byte symKey[4096];
  label = "STORAGE";
  key.assign((const char*)seed, size_seed);
  contextV.clear();
  if (!KDFa(TPM_ALG_SHA1, key, label, name, contextV, 128, 32, symKey)) {
    printf("Can't KDFa symKey\n");
    ret_val = 1;
  }
  printf("symKey: "); PrintBytes(16, symKey); printf("\n");
  printf("marshaled_credential: ");
  PrintBytes(unmarshaled_credential.size + sizeof(uint16_t),
             (byte*)&marshaled_credential);
  printf("\n");

  // encIdentity = CFBEncrypt(symKey, marshaled_credential, out)
  // We need to encrypt the entire marshaled_credential
  size_encIdentity = MAX_SIZE_PARAMS;
  if (!AesCFBEncrypt(symKey, unmarshaled_credential.size + sizeof(uint16_t),
                     (byte*)&marshaled_credential,
                     16, zero_iv,
                     &size_encIdentity, encIdentity)) {
    printf("Can't AesCFBEncrypt\n");
    goto done;
  }
  printf("\n");
  printf("size_encIdentity: %d\n", size_encIdentity);
  test_size = MAX_SIZE_PARAMS;
  if (!AesCFBDecrypt(symKey, size_encIdentity,
                     (byte*)encIdentity, 16, zero_iv,
                     &test_size, test_buf)) {
    printf("Can't AesCFBDecrypt\n");
    goto done;
  }
  printf("Decrypted secret (%d): ", test_size);
  PrintBytes(test_size, test_buf); printf("\n");

 // hmacKey= KDFa(TPM_ALG_SHA1, seed, "INTEGRITY", nullptr, nullptr, 8*hashsize);
  label = "INTEGRITY";
  if (!KDFa(TPM_ALG_SHA1, key, label, contextV,
            contextV, 8 * 20, 32, hmacKey)) {
    printf("Can't KDFa hmacKey\n");
    ret_val = 1;
    goto done;
  }

  // outerMac = HMAC(hmacKey, encIdentity || name);
  HMAC_CTX_init(&hctx);
  if (hash_alg_id == TPM_ALG_SHA1) {
    HMAC_Init_ex(&hctx, hmacKey, size_hmacKey, EVP_sha1(), nullptr);
  } else {
    HMAC_Init_ex(&hctx, hmacKey, size_hmacKey, EVP_sha256(), nullptr);
  }
  HMAC_Update(&hctx, (const byte*)encIdentity, size_encIdentity);
  HMAC_Update(&hctx, (const byte*)name.data(), name.size());
  HMAC_Final(&hctx, outerHmac, (uint32_t*)&size_hmacKey);
  HMAC_CTX_cleanup(&hctx);

done:
  return;
}
