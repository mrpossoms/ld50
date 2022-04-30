#!/usr/bin/env python

import numpy as np
import plotly
import plotly.subplots
import plotly.graph_objects as go

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
