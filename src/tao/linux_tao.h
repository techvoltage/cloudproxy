//  File: linux_tao.h
//  Author: Tom Roeder <tmroeder@google.com>
//
//  Description: LinuxTao implements the Tao for the Linux
//  operating system
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

#ifndef TAO_LINUX_TAO_H_
#define TAO_LINUX_TAO_H_

#include <map>
#include <mutex>
#include <set>
#include <string>

#include <keyczar/base/basictypes.h>  // DISALLOW_COPY_AND_ASSIGN
#include <keyczar/base/scoped_ptr.h>
#include <keyczar/base/stl_util-inl.h>  // ScopedSafeString

#include "tao/attestation.pb.h"
#include "tao/tao.h"

using std::lock_guard;
using std::map;
using std::mutex;
using std::set;
using std::string;

namespace keyczar {

class Encrypter;
class Keyczar;

}  // namespace keyczar

namespace tao {

class HostedProgramFactory;
class TaoAuth;
class TaoChannel;
class TaoChildChannel;

/// An implementation of the Tao for Linux. This implementation can take
/// different HostedProgramFactory implementations, different TaoChannel
/// implementations for communicating with its hosted programs, and different
/// TaoChildChannel implementations for communicating with its parent Tao. The
/// only assumptions LinuxTao makes are basic: it has the normal filesystem API
/// to which it can write files, and it is an intermediate Tao rather than the
/// root Tao.
class LinuxTao : public Tao {
 public:
  /// The LinuxTao is constructed with paths to keys and implementations of
  /// factories and channels.
  /// @param secret_path The location at which it stores a secret sealed by the
  /// Tao
  /// @param key_path The location of its symmetric key, encrypted to the secret
  /// @param pk_path The location of its public/private key pair, encrypted to
  /// the symmetric key
  /// @param policy_pk_path The path to the public policy key
  /// @param host_channel A channel implementation it takes ownership of and
  /// uses
  /// to communicate with its parent Tao
  /// @param child_channel A channel implementation it uses to communicat with
  /// hosted programs it starts
  /// @param program_factory A factory that creates hosted programs in the OS.
  /// @param auth_manager Handles authorization checks, including
  /// VerifyAttestation
  /// @param ca_host The name or IP address of a machine that is running tcca.
  /// If this string is empty, then the LinuxTao won't try to shorten its
  /// certification chain by communicating with tcca.
  /// @param ca_port The port to use to communicate with tcca on ca_host.
  LinuxTao(const string &secret_path, const string &key_path,
           const string &pk_path, const string &policy_pk_path,
           TaoChildChannel *host_channel, TaoChannel *child_channel,
           HostedProgramFactory *program_factory, TaoAuth *auth_manager,
           const string &ca_host, const string &ca_port);
  virtual ~LinuxTao() {}

  /// Start listening for Tao messages on channels
  bool Listen();

  // LinuxTao follows the normal semantics of the Tao for these methods
  virtual bool Init();
  virtual bool Destroy();
  virtual bool StartHostedProgram(const string &program,
                                  const list<string> &args,
				  string *identifier);
  virtual bool RemoveHostedProgram(const string &child_hash);
  virtual bool GetRandomBytes(size_t size, string *bytes) const;
  virtual bool Seal(const string &child_hash, const string &data,
                    string *sealed) const;
  virtual bool Unseal(const string &child_hash, const string &sealed,
                      string *data) const;
  virtual bool Attest(const string &child_hash, const string &data,
                      string *attestation) const;

 private:
  // create a 128-byte secret
  static const int SecretSize = 128;

  // the timeout for an Attestation (= 1 year in seconds)
  static const int AttestationTimeout = 31556926;

  // the path to the secret sealed by the Tao
  string secret_path_;

  // the path to the sealed keyczar key
  string key_path_;

  // the path to the sealed public/private keyczar key
  string pk_path_;

  // the path to the public policy key
  string policy_pk_path_;

  // keys unlocked by the secret
  scoped_ptr<keyczar::Keyczar> crypter_;

  // public/private keys unlocked by crypter_
  scoped_ptr<keyczar::Keyczar> signer_;

  // the public policy key
  scoped_ptr<keyczar::Keyczar> policy_verifier_;

  // the path to the whitelist
  string whitelist_path_;

  // A serialization of the public key of this Tao.
  string serialized_pub_key_;

  // An attestation to #serialized_pub_key_.
  Attestation pk_attest_;

  // The channel to use for host communication.
  scoped_ptr<TaoChildChannel> host_channel_;

  // A channel that handles all child connections
  scoped_ptr<TaoChannel> child_channel_;

  // A factory that can be used to start hosted programs
  scoped_ptr<HostedProgramFactory> program_factory_;

  // A class that decides whether or not a give hosted program is authorized.
  scoped_ptr<TaoAuth> auth_manager_;

  // The set of hosted programs that the LinuxTao has started
  set<string> running_children_;

  // The address:port for the TCCA. If empty, then this code won't call the CA.
  string ca_host_;
  string ca_port_;

  // a mutex for accessing the auth manager
  mutable mutex auth_m_;

  // A mutex for accessing and modifying running_children_
  mutable mutex data_m_;

  static const int AesBlockSize = 16;
  static const int Sha256Size = 32;

  /// Either unseal or create and seal a secret using the TaoChildChannel.
  bool getSecret(keyczar::base::ScopedSafeString *secret);

  /// Create a new keyset with a primary symmetric key that we will use
  /// as the basis of the Tao
  bool createKey(const string &secret);

  /// Create a new keyset with a public/private key pair to use for
  /// signing
  bool createPublicKey(keyczar::Encrypter *crypter);

  /// Get an attestation from the host Tao on our key. Note that this
  /// will get an attestation on serialized_key for this Tao host; it
  /// is for use by this Tao and its subclasses.
  bool attestToKey(const string &serialized_key, Attestation *attest);

  /// Send the attestation to the TCCA on a port/address supplied at
  /// initialization and uses the returned attestation as its attestation.
  bool getCertFromCA(Attestation *attest);

  DISALLOW_COPY_AND_ASSIGN(LinuxTao);
};
}  // namespace tao

#endif  // TAO_LINUX_TAO_H_
