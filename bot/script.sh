#!/bin/bash
if [ ! -d './temp' ]; then
  mkdir temp && cd temp || exit && \
  cp -r ../curl-8.0.1 . && \
  cd ./curl-8.0.1 || exit && \
  ./configure  --with-openssl --with-gnutls --with-mbedtls --with-schannel --with-secure-transport --with-amissl --with-bearssl --prefix="$(pwd)"/libcurl && make && \
  make install && echo "LIBCURL OK"
else
  echo LIBCURL ALREADY DONE
fi
if ! dpkg -s libbrotli-dev >/dev/null 2>&1; then
    echo "libbrotli is not installed"
    exit 1
fi
libbrotli_path=$(find /usr -name libbrotlidec.a -print -quit 2>/dev/null)
if [ -z "$libbrotli_path" ]; then
    echo "libbrotli library not found"
    exit 1
fi
