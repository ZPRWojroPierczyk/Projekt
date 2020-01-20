#!/bin/bash 
wget https://dl.bintray.com/boostorg/release/1.72.0/source/boost_1_72_0.tar.bz2
tar --bzip2 -xvf boost_1_72_0.tar.bz2
pwd=$(pwd)
rm boost_1_72_0.tar.bz2
emptyString=""
newPwd="${pwd/"/scripts"/$emptyString}" 
(cd ./boost_1_72_0 && ./bootstrap.sh --prefix=$newPwd)
(cd ./boost_1_72_0 && ./b2 install)
echo "Done!"