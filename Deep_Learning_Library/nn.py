"""
A neural network is a collection of layers.
It behaves a lot like a layer itself, although 
we're not going make it one.
"""

from typing import Sequence

from deep_learning_library.layers import Layer
from deep_learning_library.tensor import Tensor

class NeuralNet:
    def __init__(self, layers: Sequence[Layer]) -> None:
        self.layers = layers
        
    def forward(self, inputs: Tensor) -> Tensor:
        for layer in self.layers:
            inputs = layer.forward(inputs)
        return inputs
    
    def backward(self, grad: Tensor) -> Tensor:
        for layer in reversed(self.layers):
            grad = layer.backward(grad)
        return grad