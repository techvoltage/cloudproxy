#!/bin/sh
echo "Building standalone version of cloudproxy"
CGO_ENABLED=0 go install  -a -installsuffix nocgo github.com/jlmucb/cloudproxy/...
