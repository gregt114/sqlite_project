#!/bin/sh
wget https://www.sqlite.org/2023/sqlite-autoconf-3410200.tar.gz
./configure -CLFAGS="-g"
make sqlite3
