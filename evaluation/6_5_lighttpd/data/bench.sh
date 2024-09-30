#!/bin/bash

set -x
set -e

url="https://localhost:8443/index-min.py"
#url="https://localhost:8443/index.prefetchable"

configs=( "kern_patched-pf_enabled" "kern_patched-prefence_app" "kern_patched-prefence_plugin" )
#configs=( "kern_stock-pf_enabled" )
#configs=( "kern_stock-pf_disabled" )

duration="300s"
repetitions=3

for config in ${configs[@]}; do
	for (( i=0; i < $repetitions; i++ )); do
		echo Round $i
		bash run-$config.sh > lighttpd-$config-$i.log 2>&1 &
		sleep 5
		echo "httpit-$config-$i.log"
		../httpit-with-log-output/httpit --logfile "httpit-$config-$i.log" --connections 1 --duration "$duration" --insecure "$url"
		killall lighttpd
		sleep 5
	done
done
~/notify.sh "bench.sh $config done"
