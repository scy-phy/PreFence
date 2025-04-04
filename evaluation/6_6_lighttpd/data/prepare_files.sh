#!/bin/bash

set -xe

OUTDIR="./files"

gen_file () {
	dd if=/dev/urandom of="${OUTDIR}/$1_$(printf "%010d" $(($2*$3))).bin" bs=$2 count=$3
}

if [ -d $OUTDIR ]; then
	echo "${OUTDIR} exists. Aborting."
	exit 1
fi

mkdir $OUTDIR

# log10 steps up to 100 MB
for exp in {0..8}
do
	gen_file "log10" $((10**exp)) 1
done

# log2 steps up to 128 MiB
for exp in {0..27}
do
	gen_file "log2" $((2**exp)) 1
done

# steps of 16 MiB, up to 128 MiB
gen_file "16M" 1 0
for sz in $(seq $((16*1024*1024)) $((16*1024*1024)) $((128*1024*1024)))
do
	gen_file "16M" $sz 1
done