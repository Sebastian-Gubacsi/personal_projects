"""
A loss function measures how good our predictioions are,
we can use this to adjust the pereameters of our networks.
"""
import numpy as np

from deep_learning_library.tensor import tensor

class loss:
    def loss(self, predictions: tensor, actual: tensor) -> float:
        raise NotImplementedError
    
    def grad(self , predictions: tensor, actual: tensor) -> tensor:
        raise NotImplementedError
    
class MSE(loss):
    """
    MSE is mean squared error although we're 
    just going to do total squared error.
    """
    def loss(self, predictions: tensor, actual: tensor) -> float:
        return np.sum((predictions - actual) ** 2)
    
    def grad(self , predictions: tensor, actual: tensor) -> tensor:
        return 2 * (predictions - actual)