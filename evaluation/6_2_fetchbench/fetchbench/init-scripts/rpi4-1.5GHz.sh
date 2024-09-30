#!/bin/bash

set -e # abort on error
set -x # print commands

rm -rf build/
CXXFLAGS="-DARM_MSR" CFLAGS="$CXXFLAGS" ASMFLAGS="$CXXFLAGS" cmake -B build .
make -C build -j$(nproc)

cd ../covert-channel/kernel-modules/rpi4-module-ccr/armv8
sudo bash module_unload.sh
make clean
make
sudo bash module_load.sh

cd ../../../../mod-ctrlpf
sudo bash module_unload.sh
make clean
make
sudo bash module_load.sh

sudo cpufreq-set -c 0-3 -f 1.50GHz
sudo cpufreq-info
