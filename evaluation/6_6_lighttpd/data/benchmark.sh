#!/bin/bash

set -x
set -e

#configs=( "kern_patched-pf_enabled" "kern_patched-prefence_app" "kern_patched-prefence_plugin" )
configs=( "kern_stock-pf_enabled" )
#configs=( "kern_stock-pf_disabled" )

repetitions=300

for config in ${configs[@]}; do
	for (( repetition=0; repetition < $repetitions; repetition++ )); do
		echo "Round ${config}/${repetition}"
		bash "run-${config}.sh" > "lighttpd-${config}-${repetition}.log" 2>&1 &
		sleep 3
		python3 benchmark.py "${config}-${repetition}"
		killall lighttpd
		sleep 3
	done
done

if [ -f ~/notify.sh ]; then
	~/notify.sh "bench.sh $config done"
fi
