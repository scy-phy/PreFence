#!/bin/bash

set -x
set -e

url="https://localhost:8443/index-min.py"

configs=( "kern_patched-pf_enabled" "kern_patched-prefence_app" "kern_patched-prefence_plugin" )
#configs=( "kern_stock-pf_enabled" )
#configs=( "kern_stock-pf_disabled" )

rowconfigs=( 1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 )

duration="300s"
repetitions=1

for rowconfig in ${rowconfigs[@]}; do
	cd htdocs
	python3 generate_db.py -r $rowconfig
	cd ..
	for config in ${configs[@]}; do
		for (( i=0; i < $repetitions; i++ )); do
			echo Round $i
			bash run-$config.sh > lighttpd-$config-$i.log 2>&1 &
			sleep 5
			echo "httpit-$config-$i.log"
			../httpit-with-log-output/httpit --logfile "httpit-$config-$rowconfig-$i.log" --connections 1 --duration "$duration" --insecure "$url"
			killall lighttpd
			sleep 5
		done
	done
done
~/notify.sh "bench.sh $config done"
