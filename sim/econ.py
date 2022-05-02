#!/usr/bin/env python

import numpy as np
import plotly
import plotly.subplots
import plotly.graph_objects as go
import argparse

args = argparse.ArgumentParser()
args.add_argument('--method', action='store', type=int, default=0)

args = args.parse_args()

def stms():
	goods = 6
	stm = np.random.randn(goods, goods)
	print(stm)
	mask = np.random.randint(2, size=stm.shape)
	print(mask)
	stm *= mask
	np.fill_diagonal(stm, -np.abs(np.random.randn(goods)))

	x = np.random.randn(goods, 1)

	print(stm)

	X = [x]

	traces = {}

	for _ in range(500):
		x_t = X[-1]
		for i in range(x.shape[0]):
			if i not in traces:
				traces[i] = []
			else:
				traces[i].append(x_t[i][0])

		X.append(X[-1] + np.matmul(stm, X[-1]) * 0.1)

	#for i in range(x.shape[0]):
	#	print(f'{i}: {traces[i]}')

	fig = plotly.subplots.make_subplots(rows=1, cols=1, shared_xaxes=True)

	for i in range(x.shape[0]):
		fig.add_trace(
			go.Scatter(y=traces[i]),
			row=1, col=1)

	fig.show()


def foob():
	goods = 1
	
	demand = np.maximum(np.zeros(goods), np.random.randn(goods))
	
	inputs = np.abs(np.random.randn(goods, goods))
	inputs = np.fill_diagonal(inputs, 0)

	D_traces = {}
	D = [demand]
	S = [np.maximum(np.zeros(goods), np.random.randn(goods))]

	# I think supply and demand might be 2 sides of the same coin
	def supply():
		# todo
		return

	# def demand():
	# 	# todo
	# 	return

	def price(good, demand, inputs):

		# pi(x, t) = x * p(x) - C(x) - tx
		# where p(x) is the market price if it is supplied at rate x
		# C(x) is the production cost
		# x * p(x) is the revenue returned from selling
		
		# return supply / demand
		pass



	for _ in range(500):
		D_t_1 = D[-1]

		for i in range(goods):
			if i not in D_traces:
				D_traces[i] = []
			else:
				D_traces[i].append(D_t_1[i])


		# simulate demand as a random walk
		D.append(np.maximum(0, D_t_1 + np.random.randn(goods)))

	fig = plotly.subplots.make_subplots(rows=1, cols=1, shared_xaxes=True)

	D = np.array(D)
	print(D)
	# D = D.reshape(len(D), goods)

	for i in range(goods):
		fig.add_trace(
			go.Scatter(y=D_traces[i]),
			row=1, col=1)

	fig.show()

if __name__ == '__main__':

	methods = {
		0: stms,
		1: foob
	}

	methods[args.method]()