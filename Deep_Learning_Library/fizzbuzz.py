"""
FizzBuzz is the following problem:

for each of the numbers 1 to 100:
- if the number is divisible by 3, print "Fizz"
- if the number is divisible by 5, print "Buzz"
- if the number is divisible by 15, print "FizzBuzz"
- otherwise, print the number
"""

import numpy as np
from typing import List

from Deep_Learning_Library.train import train
from Deep_Learning_Library.nn import NeuralNet
from Deep_Learning_Library.layers import Linear, Tanh
from Deep_Learning_Library.optim import SGD

def fizz_buzz_encode(x: int) -> List[int]:
    if x % 15 == 0:
        return [0, 0, 0, 1]  # FizzBuzz
    elif x % 5 == 0:
        return [0, 0, 1, 0]  # Buzz
    elif x % 3 == 0:
        return [0, 1, 0, 0]  # Fizz
    else:
        return [1, 0, 0, 0]  # Number
    
def binary_encode(x: int) -> List[int]:
    """
    10 digit binary encoding of x
    """
    return [x >> i & 1 for i in range(10)]

inputs = np.array([
    binary_encode(x) 
    for x in range(101, 1024)
])

targets = np.array([
    fizz_buzz_encode(x) 
    for x in range(101, 1024)
])

net = NeuralNet([
    Linear(input_size=10, output_size=50),
    Tanh(),
    Linear(input_size=50, output_size=4)
])

train(
    net,
    inputs,
    targets,
    epochs=5000,
    optim=SGD(lr=0.001)
)

for x in range(1, 101):
    predicted = net.forward(binary_encode(x))
    predicted_idx = np.argmax(predicted)
    actual_idx = np.argmax(fizz_buzz_encode(x))
    labels = [str(x), "Fizz", "Buzz", "FizzBuzz"]
    print(x, labels[predicted_idx], labels[actual_idx])