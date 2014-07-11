//  File: client.cc
//  Author: Tom Roeder <tmroeder@google.com>
//
//  Description: An example client application using CloudClient
//
//  Copyright (c) 2013, Google Inc.  All rights reserved.
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
#include <string>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <openssl/ssl.h>

#include "cloudproxy/cloud_client.h"
#include "cloudproxy/cloud_user_manager.h"
#include "cloudproxy/cloudproxy.pb.h"
#include "tao/keys.h"
#include "tao/tao_child_channel.h"
#include "tao/tao_child_channel_registry.h"
#include "tao/tao_domain.h"
#include "tao/util.h"

using std::string;

using cloudproxy::CloudClient;
using cloudproxy::CloudUserManager;
using cloudproxy::ScopedSSL;
using tao::Base64WDecode;
using tao::Base64WEncode;
using tao::InitializeApp;
using tao::Keys;
using tao::TaoChildChannel;
using tao::TaoChildChannelRegistry;
using tao::TaoDomain;

DEFINE_string(config_path, "tao.config", "Location of tao configuration");
DEFINE_string(client_keys, "./client_keys",
              "Directory for client keys and TLS files");
DEFINE_string(user, "tmroeder", "Name of user to authenticate");
DEFINE_string(user_keys, "./user_keys", "Location of user keys");
DEFINE_string(address, "localhost", "The address of the local server");
DEFINE_string(port, "11235", "The server port to connect to");

int main(int argc, char** argv) {
  InitializeApp(&argc, &argv, false);

  // the last argument should be the parameters for channel establishment
  if (argc < 2) {
    LOG(ERROR) << "Too few arguments to client";
    return 1;
  }

  // The convention is that the last argument in a process-based hosted program
  // is the child channel params encoded as Base64W.
  string encoded_params(argv[argc - 1]);
  string params;
  if (!Base64WDecode(encoded_params, &params)) {
    LOG(ERROR) << "Could not decode the encoded params " << encoded_params;
    return 1;
  }

  TaoChildChannelRegistry registry;
  tao::RegisterKnownChannels(&registry);

  unique_ptr<TaoChildChannel> channel(registry.Create(params));
  CHECK(channel->Init()) << "Could not initialize the child channel";

  unique_ptr<TaoDomain> admin(TaoDomain::Load(FLAGS_config_path));
  CHECK(admin.get() != nullptr) << "Could not load configuration";

  int size = 6;
  string name_bytes;
  CHECK(channel->GetRandomBytes(size, &name_bytes))
      << "Could not get a random name from the Tao";

  int policy = 0;  // TODO(kwalsh) chose policy here
  CloudClient cc(FLAGS_client_keys, channel.release(), policy, admin.release());
  CHECK(cc.Init());

  ScopedSSL ssl;
  CHECK(cc.Connect(FLAGS_address, FLAGS_port, &ssl))
      << "Could not connect to the server at " << FLAGS_address << ":"
      << FLAGS_port;

  // create a random object name to write, getting randomness from the Tao

  // Base64 encode the bytes to get a printable name
  string name;
  CHECK(Base64WEncode(name_bytes, &name)) << "Could not encode name";

  unique_ptr<Keys> key;
  string user = FLAGS_user;
  string password = FLAGS_user;
  CHECK(CloudUserManager::LoadUser(FLAGS_user_keys, user, password, &key));
  CHECK(cc.AddUser(user, *key->Signer())) << "Could not add the user key";
  CHECK(cc.Authenticate(ssl.get(), user,
                        key->GetPath(CloudUserManager::UserDelegationSuffix)))
      << "Could not authenticate user with the server";
  LOG(INFO) << "Authenticated to the server for " << user;
  CHECK(cc.Create(ssl.get(), user, name)) << "Could not create object"
                                          << "'" << name << "' on the server";
  CHECK(cc.Read(ssl.get(), user, name, name)) << "Could not read the object";
  CHECK(cc.Destroy(ssl.get(), user, name)) << "Could not destroy the object";
  LOG(INFO) << "Created, Read, and Destroyed the object " << name;

  CHECK(cc.Close(ssl.get(), false)) << "Could not close the channel";

  LOG(INFO) << "Test succeeded";

  return 0;
}
