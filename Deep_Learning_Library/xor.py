"""
The cannnocial example of a function that can't be 
learned with a simple linear model is XOR
"""

import numpy as np

from Deep_Learning_Library.train import train
from Deep_Learning_Library.nn import NeuralNet
from Deep_Learning_Library.layers import Linear, Tanh

inputs = np.array([
    [0, 0],
    [1, 0],
    [0, 1],
    [1, 1]
])

target = np.array([
    [1, 0],
    [0, 1],
    [0, 1],
    [1, 0]
])

net = NeuralNet([
    Linear(input_size=2, output_size=2), 
    Tanh(),
    Linear(input_size=2, output_size=2)
])

train(net, inputs, target)

for x, y in zip(inputs, target, net.forward(inputs)):
    predicted = net.forward(x)
    print(x, predicted, y)