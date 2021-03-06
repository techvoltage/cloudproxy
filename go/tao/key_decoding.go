//  Copyright (c) 2017, John Manferdelli, All rights reserved.
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
// limitations under the License.

package tao

import (
	"crypto/x509"
)

func ptrFromString(str string) *string {
	return &str
}

func PublicKeyAlgFromSignerAlg(signerAlg string) int {
	switch(signerAlg) {
	case "ecdsap256", "ecdsap384", "ecdsap521":
		return int(x509.ECDSA)
	case "rsa1024", "rsa2048", "rsa3072":
		return int(x509.RSA)
	default:
		return -1
	}
	return -1
}

func SignatureAlgFromSignerAlg(signerAlg string) int {
	switch(signerAlg) {
	case "ecdsap256", "ecdsap384", "ecdsap521":
		return int(x509.ECDSAWithSHA256)
	case "rsa1024", "rsa2048", "rsa3072":
		return int(x509.SHA256WithRSA)
	default:
		return -1
	}
	return -1
}

func CrypterTypeFromSuiteName(suiteName string) *string {
	switch suiteName {
	case Basic128BitCipherSuite:
		return ptrFromString("aes128-ctr-hmacsha256")
	case Basic192BitCipherSuite:
		return ptrFromString("aes256-ctr-hmacsha384")
	case Basic256BitCipherSuite:
		return ptrFromString("aes256-ctr-hmacsha512")
	default:
		return nil
	}
	return nil
}

func SignerTypeFromSuiteName(suiteName string) *string {
	switch suiteName {
	case Basic128BitCipherSuite:
		return ptrFromString("ecdsap256")
	case Basic192BitCipherSuite:
		return ptrFromString("ecdsap384")
	case Basic256BitCipherSuite:
		return ptrFromString("ecdsap521")
	default:
		return nil
	}
	return nil
}

func VerifierTypeFromSuiteName(suiteName string) *string {
	n := SignerTypeFromSuiteName(suiteName)
	if n == nil {
		return nil
	}
	return ptrFromString(*n + "-public")
}

func DeriverTypeFromSuiteName(suiteName string) *string {
	switch suiteName {
	case Basic128BitCipherSuite, Basic192BitCipherSuite, Basic256BitCipherSuite:
		return ptrFromString("hdkf-sha256")
	/*
	// Replace later?
	case Basic192BitCipherSuite:
		return ptrFromString("hdkf-sha384")
	case Basic256BitCipherSuite:
		return ptrFromString("hdkf-sha512")
	 */
	default:
		return nil
	}
	return nil
}

func HmacTypeFromSuiteName(suiteName string) *string {
	switch suiteName {
	case Basic128BitCipherSuite:
		return ptrFromString("hmacsha256")
	case Basic192BitCipherSuite:
		return ptrFromString("hmacsha384")
	case Basic256BitCipherSuite:
		return ptrFromString("hmacsha512")
	default:
		return nil
	}
	return nil
}

func CipherTypeFromSuiteName(suiteName string) *string {
	switch suiteName {
	case Basic128BitCipherSuite:
		return ptrFromString("aes128-ctr")
	case Basic192BitCipherSuite, Basic256BitCipherSuite:
		return ptrFromString("aes256-ctr")
	default:
		return nil
	}
	return nil
}

func HashTypeFromSuiteName(suiteName string) *string {
	switch suiteName {
	case Basic128BitCipherSuite:
		return ptrFromString("sha256")
	case Basic192BitCipherSuite:
		return ptrFromString("sha384")
	case Basic256BitCipherSuite:
		return ptrFromString("sha512")
	default:
		return nil
	}
	return nil
}

func IsSinger(keyType string) bool {
	switch(keyType) {
	default:
		return false
	case "rsa1024", "rsa2048", "rsa3072",
	     "ecdsap256", "ecdsap384", "ecdsap521":
		return true
	}
	return false
}

func IsCrypter(keyType string) bool {
	switch(keyType) {
	default:
		return false
	case "aes128-ctr-hmacsha256", "aes256-ctr-hmacsha256":
		return true
	}
	return false
}

func IsDeriver(keyType string) bool {
	switch(keyType) {
	default:
		return false
	case "hdkf-sha256":
		return true
	}
	return false
}

func SymmetricKeySizeFromAlgorithmName(keyType string) *int {
	var n int
	switch(keyType) {
	default:
		return nil
	case "aes128-ctr-hmacsha256":
		n = 16
		return &n
	case "aes256-ctr-hmacsha384", "aes256-ctr-hmacsha512":
		n = 32
		return &n
	}
	return nil
}

func HmacSizeFromAlgorithmName(keyType string) *int {
	var n int
	switch(keyType) {
	default:
		return nil
	case "aes128-ctr-hmacsha256":
		n = 32
		return &n
	case "aes256-ctr-hmacsha384":
		n = 48
		return &n
	case "aes256-ctr-hmacsha512":
		n = 64
		return &n
	}
	return nil
}

func HmacKeySizeFromAlgorithmName(keyType string) *int {
	var n int
	switch(keyType) {
	default:
		return nil
	case "aes128-ctr-hmacsha256":
		n = 32
		return &n
	case "aes256-ctr-hmacsha384":
		n = 48
		return &n
	case "aes256-ctr-hmacsha512":
		n = 64
		return &n
	}
	return nil
}

func CombinedKeySizeFromAlgorithmName(keyType string) *int {
	var n int
	switch(keyType) {
	default:
		return nil
	case "aes128-ctr-hmacsha256":
		n = 48
		return &n
	case "aes256-ctr-hmacsha384":
		n = 64
		return &n
	case "aes256-ctr-hmacsha512":
		n = 96
		return &n
	}
	return nil
}

func SymmetricBlockSizeFromAlgorithmName(keyType string) *int {
	var n int
	switch(keyType) {
	default:
		return nil
	case "aes128-ctr-hmacsha256", "aes256-ctr-hmacsha384", "aes256-ctr-hmacsha512",
		"aes128-raw", "aes256-raw":
		n = 16
		return &n
	}
	return &n
}
