#!/bin/bash
if [ ! -d './temp' ]; then
  mkdir temp && cd temp || exit && \
  cp -r ../curl-8.0.1 . && \
  cd ./curl-8.0.1 || exit && \
  ./configure --without-ssl --prefix="$(pwd)"/libcurl && make && \
  make install && echo "LIBCURL OK"
else
  echo LIBCURL ALREADY DONE
fi
# Check if libbrotli is installed
if ! dpkg -s libbrotli-dev >/dev/null 2>&1; then
    echo "libbrotli is not installed"
    exit 1
fi
# Find the path to the libbrotli library
libbrotli_path=$(find /usr -name libbrotlidec.a -print -quit 2>/dev/null)

# Check if the libbrotli library was found
if [ -z "$libbrotli_path" ]; then
    echo "libbrotli library not found"
    exit 1
fi


