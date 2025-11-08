"""
Our neural nets will be made up of layers.
Each layer needs to pass its input forwrad 
and propigate gradients backward. For example, 
a neural net mmight look like this:

inputs -> Linear -> Tanh -> Linear -> output
"""
from typing import Dict, Callable
import numpy as np

from deep_learning_library.tensor import Tensor

class Layer:
    def __init__(self) -> None:
        self.params: Dict[str, Tensor] = {}
        self.grads: Dict[str, Tensor] = {}
    
    def forward(self, inputs: Tensor) -> Tensor:
        """
        Product the outputs corresponding to the inputs.
        """
        raise NotImplementedError
    
    def backward(self, gard: Tensor) -> Tensor:
        """
        Backpropigate this gradient through the layer.
        """
        raise NotImplementedError
    
class Linear(Layer):
    """
    computes outputs = inputs @ w + b
    """
    def __init__(self, input_size: int, output_size: int) -> None:
        # inputs will be batch size by input size
        # outputs will be batch size by output size
        super().__init__()
        self.params["w"] = np.random.randn(input_size, output_size)
        self.params["b"] = np.random.randn(output_size)
    
    def forward(self, inputs: Tensor) -> Tensor:
        """
        outputs = inputs @ w + b
        """
        self.inputs = inputs
        return inputs @ self.params["w"] + self.params["b"]
    
    def backward(self, grad: Tensor) -> Tensor:
        """
        if y = f(x) and x = a * b + c
        then dy/da = f'(x) * b
        and dy/db = f'(x) * a
        and dy/dc = f'(x)
        
        if y = f(x) and x = a @ b + c
        then dy/da = f'(x) @ b.T
        and dy/db = a.T @ f'(x)
        and dy/dc = f'(x)
        """
        self.grads["b"] = np.sum(grad, axis=0)
        self.grads["w"] = self.inputs.T @ grad
        return grad @ self.params["w"].T

F = callable[[Tensor], Tensor]

class Activation(Layer):
    """
    An activiation layer just applies a function 
    elementwise to its inputs.
    """
    def __init__(self, f: F, f_prime:F) -> None:
        super().__init__()
        self.f = f
        self.f_prime = f_prime
        
    def forward(self, inputs: Tensor) -> Tensor:
        self.input = inputs
        return self.f(inputs)
    
    def backward(self, grad: Tensor) -> Tensor:
        """
        if y = f(x) and x = g(z)
        then dy/dz = f'(x) * g'(z)
        
        f'(x) is the gradient of the activation function
        g'(z) is the gradient coming from the next layer
        """
        return grad * self.f_prime(self.inputs)
    
def tanh(x: Tensor) -> Tensor:
    return np.tanh(x)

def tanh_prime(x: Tensor) -> Tensor:
    y = np.tanh(x)
    return 1 - y ** 2

class Tanh(Activation):
    def __init__(self) -> None:
        super().__init__(tanh, tanh_prime)