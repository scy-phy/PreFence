#!/usr/bin/env python3

import sqlite3
import random

DB_FILE = "database.db"
TABLE_NAME = "data"

###

# open db file
con = sqlite3.connect(DB_FILE)
cur = con.cursor()

# get column names
query_column_names = f"SELECT name FROM PRAGMA_TABLE_INFO(?);"
result_column_names = cur.execute(query_column_names, (TABLE_NAME,))
column_names = []
for result in result_column_names:
	column_names.append(result[0])

# select a column
column_name = random.choice(column_names)

# find minimum value in selected column
query_min = f"SELECT min({column_name}) FROM {TABLE_NAME};"
result_min = cur.execute(query_min)
min_value = cur.fetchone()[0]

con.close()

print("Content-Type: text/html;charset=utf-8\n")
print(f"""<!doctype html>
<html>
	<head>
		<meta charset="utf-8" />
		<title>Result</title>
	</head>
	<body>
		<h1>Result</h1>
		<table border="1">
			<tr>
				<td>Selected column</td>
				<td><code>{column_name}</code></td>
			</tr>
			<tr>
				<td>Minimum value</td>
				<td><code>{min_value}</code></td>
			</tr>
		</table>
	</body>
</html>""")
