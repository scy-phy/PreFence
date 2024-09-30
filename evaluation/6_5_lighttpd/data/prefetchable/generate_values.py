#!/usr/bin/env python3

import random

NO_COLUMNS = 16
NO_ROWS = 1 * 1024

def get_random_value() -> int:
	return random.getrandbits(32)

output = "#pragma once\n"
output += f"#define NO_COLUMNS ({NO_COLUMNS})\n"
output += f"#define NO_ROWS ({NO_ROWS})\n"
output += "uint32_t values[NO_ROWS][NO_COLUMNS] = {\n"

for row_idx in range(NO_ROWS):
	output += "\t{ "
	for col_idx in range(NO_COLUMNS):
		output += f"0x{get_random_value():08x}, "
	output += "},\n"
output += "};\n"
print(output)
