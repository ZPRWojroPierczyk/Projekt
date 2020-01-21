#!/bin/bash

# Get project's directory
PROJECT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

# Download sqlite3
cd $PROJECT_DIR/external
wget https://www.sqlite.org/2019/sqlite-autoconf-3300100.tar.gz
tar xvfz sqlite-autoconf-3300100.tar.gz
rm sqlite-autoconf-3300100.tar.gz
# Install sqlite3
mkdir build
mkdir $PROJECT_DIR/include/sqlite3
mkdir $PROJECT_DIR/lib/sqlite3
cd sqlite-autoconf-3300100
./configure --prefix=$PROJECT_DIR/external/build --libdir=$PROJECT_DIR/lib/sqlite3 --includedir=$PROJECT_DIR/include/sqlite3
make
make install
cd $PROJECT_DIR
rm -r external/sqlite-autoconf-3300100
rm -r external/build