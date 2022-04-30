#!/usr/bin/env python

import numpy as np
import plotly
import plotly.subplots
import plotly.graph_objects as go


stm = np.random.randn(6, 6)
x = np.random.randn(6, 1)

X = [x]

traces = {}

for _ in range(200):
	x_t = X[-1]
	for i in range(x.shape[0]):
		print(i)
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
