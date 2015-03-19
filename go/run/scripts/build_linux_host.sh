#!/bin/bash

# This script assumes that the binary for linux_host has been built and
# installed into ${BINDIR}/linux_host. For the purposes of KVM/CoreOS, this
# binary must be executable on the virtual machine. One way to make this easier
# is to build the binary statically. E.g., see run/scripts/build_standalone.sh.

if [ "$#" != "2" ]; then
	echo "Must supply a policy certificate and a tao.config for the VM linux_host"
	exit 1
fi

BINDIR=${GOPATH}/bin
TEMP_DIR=`mktemp -d`
cp ${BINDIR}/linux_host ${TEMP_DIR}/linux_host
mkdir ${TEMP_DIR}/policy_keys
mkdir ${TEMP_DIR}/linux_tao_host
chmod 755 ${TEMP_DIR}/linux_tao_host
cp $1 ${TEMP_DIR}/policy_keys/cert
cp $2 ${TEMP_DIR}/tao.config

tar -C ${TEMP_DIR} -czf ${BINDIR}/linux_host.img.tgz `ls ${TEMP_DIR}`
rm -fr ${TEMP_DIR}