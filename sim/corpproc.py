#!/usr/bin/env python

import re
import argparse

args = argparse.ArgumentParser()
args.add_argument('--corpus', action='store', type=str)
args = args.parse_args()


with open(args.corpus) as f:
	line = f.readline()

	while line is not None and len(line) > 0:
		m = re.search("'''\[\[\w+\]\]'''", line)

		if m is not None:
			name = m.group(0).replace("'''[[", '').replace("]]'''", '').lower()
			print(name)

		line = f.readline()