"""
here's a function that can train a neural net.
"""

from deep_learning_library.Tensor import Tensor
from deep_learning_library.nn import NeuralNet
from deep_learning_library.loss import Loss, MSE
from deep_learning_library.optim import Optimizer, SGD
from deep_learning_library.data import DataIterator, BatchIterator

def train(net: NeuralNet, inputs: Tensor, Targets: Tensor, num_epochs: int = 5000, iterator: DataIterator = BatchIterator(), loss: Loss = MSE(), optim: Optimizer = SGD()) -> None:
    for epoch in range(num_epochs):
        epoch_loss = 0.0
        for batch in iterator(inputs, Targets):
            predidcted = net.forward(batch.inputs)
            epoch_loss += loss.forward(predidcted, batch.targets)
            grad = loss.grad(predidcted, batch.targets)
            net.backward(grad)
            Optimizer.step(net)
        print(epoch, epoch_loss)
            