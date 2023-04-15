#!/bin/sh

# Compile flags
cflags="-g -fsanitize=address -fno-omit-frame-pointer -fstack-protector"
ldflags="-fsanitize=address"

# Download sqlite source amalgamation
if ! which wget > /dev/null; then
	echo "wget not found, install wget and try again"
	exit 1
fi
wget https://www.sqlite.org/2023/sqlite-autoconf-3410200.tar.gz

# Extract archive
tar -x --file=sqlite-autoconf-3410200.tar.gz || {
	echo "Error extracting archive"
	exit 1
}

# Compile with debug symbols and address sanitizer
cd sqlite-autoconf-3410200
./configure CFLAGS="$cflags" LDFLAGS="$ldflags" --enable-debug
make

