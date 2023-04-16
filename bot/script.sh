#!/bin/bash
mkdir temp && cd temp || exit && \
cp -r ../curl-8.0.1 . && \
cd ./curl-8.0.1 || exit && \
./configure --without-ssl --prefix="$(pwd)"/libcurl && make && \
make install && echo "LIBCURL OK"