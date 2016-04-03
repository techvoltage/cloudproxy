// Copyright (c) 2014, Google, Inc. All rights reserved.
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
//

package main

import (
	"flag"
	"fmt"
	"io/ioutil"

	"github.com/jlmucb/cloudproxy/go/tpm2"
)

// This program creates a key hierarchy consisting of a
// primary key, and quoting key for cloudproxy
// and makes their handles permanent.
func main() {
	keySize := flag.Int("modulus size",  2048, "Modulus size for keys")
	policyKeyFile:= flag.String("Policy save file",  "../tmptest/policy.go.bin",
		"policy save file")
	policyKeyPassword := flag.String("Policy key password",  "xxzzy",
		"policy key password")
	flag.Parse()

	// Open tpm
	rw, err := tpm.OpenTPM("/dev/tpm0")
	if err != nil {
		fmt.Printf("OpenTPM failed %s\n", err)
		return
	}
	defer rw.Close()

	// Flushall
	err =  tpm.Flushall(rw)
	if err != nil {
		fmt.Printf("Flushall failed\n")
		return
	}
	fmt.Printf("rw: %x\n", rw)

}
