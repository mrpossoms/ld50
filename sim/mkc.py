#!/usr/bin/env python

import numpy as np
import plotly
import plotly.subplots
import plotly.graph_objects as go
import argparse
import random

args = argparse.ArgumentParser()
args.add_argument('--corpus', action='store', type=str)
args.add_argument('--genname', action='store_true')
args.add_argument('--genmatrix', action='store_true')

args = args.parse_args()

def markov_chain(file):
	l = (ord('Z') - ord('0')) + 1
	M = np.zeros((l, l))
	N = 0
	len_hist = {}

	line = file.readline()
	while line is not None and len(line) > 0:
		line = line.strip().upper()
		for ci in range(len(line) - 1):
			M[ord(line[ci]) - ord('0')][ord(line[ci+1]) - ord('0')] += 1

		if len(line) not in len_hist:
			len_hist[len(line)] = 0
		else:
			len_hist[len(line)] += 1

		line = file.readline()
		N += 1

	# normalize probability of given name len_hist
	lengths = []
	for k in len_hist:
		len_hist[k] /= N
		lengths.append(k)

	alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
	vowels = 'AEIOU'
	n = random.choice(lengths)
	name = ''
	i = ord(random.choice(alphabet)) - ord('0')

	if args.genname:
		for _ in range(n):
			name += chr(i + ord('0'))
			# print(M[i])

			if random.random() < 0.25:
				no_vowels = True
				for c in vowels:
					no_vowels |= (c in name)
				
				if no_vowels:
					i = ord(random.choice(vowels)) - ord('0')
				else:
					i = ord(random.choice(alphabet)) - ord('0')
			else:
				i = np.argmax(M[i])
		print(name)

	if args.genmatrix:
		print(f'unsigned char_prob[{M.shape[0]}][{M.shape[1]}] = {{')
		for r in range(M.shape[0]):
			row = ''
			for c in M[r]:
				row += f'{int(c)}, '

			print(f'{{ {row} }},')
		print('};')

if __name__ == '__main__':

	with open(args.corpus) as f:
		markov_chain(f)