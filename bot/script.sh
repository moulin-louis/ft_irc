#!/bin/bash
if ! dpkg -s libbrotli-dev >/dev/null 2>&1; then
  echo "libbrotli is not installed"
  exit 1
else
  echo "libbrotli ok"
fi
libbrotli_path=$(find /usr -name libbrotlidec.a -print -quit 2>/dev/null)
if [ -z "$libbrotli_path" ]; then
  echo "libbrotlidec.a library not found"
  exit 1
else
  echo "libbrotlidec.a ok"
fi
if [ ! -d './temp' ]; then
  mkdir -p temp && cd temp || exit && \
  cp -r ../curl-8.0.1 . && \
  cd ./curl-8.0.1 || exit && \
  ./configure --with-openssl \
              --with-gnutls \
              --with-mbedtls \
              --with-schannel \
              --with-secure-transport \
              --with-amissl \
              --with-bearssl \
              --prefix="$(pwd)"/libcurl && \
  make && \
  make install && echo "libcurl ok"
else
  echo "libcurl already done"
fi
