#!/bin/bash

set -e # abort on error
set -x # print commands

rm -rf build/
CXXFLAGS="-DRDTSC" CFLAGS="$CXXFLAGS" ASMFLAGS="$CXXFLAGS" cmake -B build .
make -C build -j$(nproc)

sudo modprobe msr

cd ../mod-ctrlpf
sudo bash module_unload.sh
make clean && make
sudo bash module_load.sh
cd ../fetchbench

sudo cpupower -c all frequency-set -f 2.70GHz
sudo cpupower -c all frequency-info