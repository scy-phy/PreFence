import gzip
import os
import re
import glob

### Parsing

def parse_file(path: str) -> float:
	print(f"[{path:36s}] Parsing...")
	
	with gzip.open(path, "rb") as file:
		start = file.read(1024).decode()
		file.seek(-2048, os.SEEK_END)
		end = file.read().decode()

	rx_start = re.match(r"Timestamp \(program start\): (?P<timestamp>\d+)", start)
	if rx_start:
		start_int = int(rx_start.group("timestamp"))

	end = end.splitlines()[-1]
	rx_end = re.match(r"Timestamp \(program end\): (?P<timestamp2>\d+)", end)
	if rx_end:
		end_int = int(rx_end.group("timestamp2"))
	
	delta = end_int - start_int
	print(delta, delta/60)
	return delta / 60

results_with_prefetch = []
for filepath in glob.glob("./with_prefetch/out-with_prefetch-*.log.gz"):
	results_with_prefetch.append(parse_file(filepath))

results_without_prefetch = []
for filepath in glob.glob("./without_prefetch/out-without_prefetch-*.log.gz"):
	results_without_prefetch.append(parse_file(filepath))

results_all = results_with_prefetch + results_without_prefetch

print("results_with_prefetch", results_with_prefetch)
print("mean(results_with_prefetch)", sum(results_with_prefetch)/len(results_with_prefetch))
print("results_without_prefetch", results_without_prefetch)
print("mean(results_without_prefetch)", sum(results_without_prefetch) / len(results_without_prefetch))
print("results_all", results_all)
print("mean(results_all)", sum(results_all) / len(results_all))
