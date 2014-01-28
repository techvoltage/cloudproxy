#!/bin/bash
# Copyright (c) 2014, Google Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
if [[ "$#" != "2" ]]; then
  echo "Usage: $0 <test dir> <build dir>"
  exit 1
fi

TEST=$1
BUILD_DIR=$2

cd ${TEST}
mkdir fake_key
${BUILD_DIR}/keyczart create --location=fake_key --purpose=sign \
  --asymmetric=ecdsa
${BUILD_DIR}/keyczart addkey --location=fake_key --status=primary

# The default flags work in our setup
${TEST}/attest_to_key
