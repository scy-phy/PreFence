#/bin/bash

set -x
set -e

for i in {0..2}
do
	build/victim-bn 02468ace 9 > results_$i.log
done