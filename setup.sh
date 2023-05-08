#!/bin/sh

# Compile flags
cflags="-g -fsanitize=address -fno-omit-frame-pointer -fstack-protector"
ldflags="-fsanitize=address"

# Download sqlite source amalgamation
if ! which wget > /dev/null; then
	echo "ERROR: wget not found, install wget and try again"
	exit 1
fi
wget https://www.sqlite.org/2023/sqlite-autoconf-3410200.tar.gz

# Extract archive
tar -x --file=sqlite-autoconf-3410200.tar.gz || {
	echo "Error extracting archive"
	exit 1
}

# Check for AFL++ installation
if ! which afl-fuzz > /dev/null; then
	echo "ERROR: AFL++ not found, install AFL++ and try again"
	exit 1
fi

# Compile with debug symbols, address sanitizer, and AFL++ instrumentation
cd sqlite-autoconf-3410200
./configure CFLAGS="$cflags" LDFLAGS="$ldflags" --enable-debug CC=/usr/local/bin/afl-gcc CXX=/usr/local/bin/afl-g++
make
cd ..

# Download sqlsmith, NB don't use the debian version, it is outdated
git clone https://github.com/anse1/sqlsmith.git
cd sqlsmith
autoreconf -i
./configure
make
cd ..
