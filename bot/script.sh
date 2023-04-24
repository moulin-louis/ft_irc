#!/bin/bash
curl -Os https://curl.se/download/curl-8.0.1.tar.gz && \
tar -xf curl-8.0.1.tar.gz && \
rm -f curl-8.0.1.tar.gz && \
echo "libcurl downloaded successfuly"
libbrotli_path=$(find /usr -name libbrotlidec.a -print -quit 2>/dev/null)
libssl_path=$(find /usr -name libssl.a -print -quit 2>/dev/null)
libcrypto_path=$(find /usr -name libcrypto.a -print -quit 2>/dev/null)
if [ -z "$libbrotli_path" ]; then
  echo "libbrotlidec.a library not found"
  exit 1
else
  echo "libbrotlidec.a ok"
fi
if [ -z "$libssl_path" ]; then
  echo "libssl.a library not found"
  exit 1
else
  echo "libssl.a ok"
fi
if [ -z "$libcrypto_path" ]; then
  echo "libcrypto.a library not found"
  exit 1
else
  echo "libcrypto.a ok"
fi
if [ ! -d './temp' ]; then
  mkdir -p temp && cd temp || exit && \
  cp -r ../curl-8.0.1 . && \
  cd ./curl-8.0.1 || exit && \
  ./configure --disable-test \
              --disable-manual \
              --with-openssl \
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
