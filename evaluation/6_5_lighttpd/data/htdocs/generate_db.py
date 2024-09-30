#!/usr/bin/env python3

import os
import sqlite3
import random
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-r", "--rows", required=True)
args = parser.parse_args()

NO_COLUMNS = 16
NO_ROWS = int(args.rows)
DB_FILE = "database.db"
TABLE_NAME = "data"
random.seed(0x781a29f03e4c)

#####

def get_random_value() -> int:
	# regular pattern for debug purposes
	# return (random.getrandbits(32) & 0x0000ffff) | 0xffee0000
	# ensure the highest bit is always set -> constant width
	return 0x80000000 | random.getrandbits(32)

#####

# remove db file if it exists
if os.path.isfile(DB_FILE):
	os.unlink(DB_FILE)

# open (new) db file
con = sqlite3.connect(DB_FILE)
cur = con.cursor()

# create table with NO_COLUMNS columns
query_create_table = f"CREATE TABLE {TABLE_NAME}("
for col_idx in range(NO_COLUMNS):
	query_create_table += f"col{col_idx:06d} INTEGER"
	query_create_table += ", "
query_create_table = query_create_table[:-2]
query_create_table += ");"
cur.execute(query_create_table)

# insert NO_ROWS rows containing random numbers
for row_idx in range(NO_ROWS):
	query_insert = "INSERT INTO data VALUES("
	for col_idx in range(NO_COLUMNS):
		query_insert += str(get_random_value())
		query_insert += ", "
	query_insert = query_insert[:-2]
	query_insert += ");"
	cur.execute(query_insert)
	#con.commit()
con.commit()

con.close()
