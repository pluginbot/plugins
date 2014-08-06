#! /bin/bash

echo "Building zLib"
./zlib/configure
make -w -C ./zlib
echo "Done building zLib"
