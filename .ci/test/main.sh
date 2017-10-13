#!/usr/bin/env bash

set -ex

cd build/test
./ReplicationTest
./DeleteTest
./NameNodeTest
./NativeFsTest
./UsernameTest
./ZKDNClientTest
./ZKLockTest
./ZKWrapperTest
./ReadWriteTest
