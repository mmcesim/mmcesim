#!/bin/sh

## This script builds and packs mmCEsim CLI for macOS with ARM architecture.
## This is useful since there is no ARM support for macOS on Github Actions.
## The binary will be installed to ./usr/mmcesim, as in GitHub Actions.

#------------------------------
# Definition
#------------------------------
mmCEsim_install_dir=usr/mmcesim
mmCEsim_arch=arm

#------------------------------
# Build and Install
#------------------------------
mkdir build
cmake . build -D CMAKE_INSTALL_PREFIX=${mmCEsim_install_dir} -D MMCESIM_APPLE_COPY_SH=ON
cd build
make
make install
cd ..

#------------------------------
# Pack
#------------------------------
mkdir publish
cd ${mmCEsim_install_dir}
cmake -E tar cfv ../../publish/mmCEsim-CLI-macOS-${mmCEsim_arch}.zip --format=zip .

#------------------------------
# Finish
#------------------------------
echo "Pack finished!"
