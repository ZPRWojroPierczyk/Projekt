#!/bin/bash

# Get project's directory
PROJECT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

# Download boost to the /external folder
wget https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.bz2
tar --directory=$PROJECT_DIR/external --bzip2 -xvf boost_1_72_0.tar.bz2
rm boost_1_72_0.tar.bz2
mkdir $PROJECT_DIR/lib/boost
cd $PROJECT_DIR/external/boost_1_72_0
# Configure bootstrap 
./bootstrap.sh \
    --prefix=$PROJECT_DIR \
    --exec-prefix=$PROJECT_DIR \
    --libdir=$PROJECT_DIR/lib/boost \
    --includedir=$PROJECT_DIR/include \
    --with-libraries=system,program_options,test
# Compile boost
./b2 install\
    --prefix=$PROJECT_DIR \
    --exec-prefix=$PROJECT_DIR \
    --libdir=$PROJECT_DIR/lib/boost \
    --includedir=$PROJECT_DIR/include \
    --with-system --with-program_options --with-test