#搭建神经网络：
import torch
from torch import nn


class Neet(nn.Module):#构建简单的网络
    def __init__(self) -> None:
        super().__init__()
        self.model = nn.Sequential(
        nn.Conv2d(1,10,kernel_size=5),
        nn.MaxPool2d(2),
        nn.ReLU(),
        nn.Conv2d(10,20,kernel_size=5),
        nn.Dropout2d(),
        nn.MaxPool2d(2),
        nn.ReLU(),
        )
        self.fc1 = nn.Linear(320,50)
        self.fc2 = nn.Linear(50,10)
        self.fc3 = nn.Linear(10,5)
    def forward(self,x):
        x = self.model(x)
        x = x.view(-1,320)
        x = self.fc1(x)
        x = self.fc2(x)
        x = self.fc3(x)
        return x

# if __name__ == '__main__':
#     mode = Mode()
#     input = torch.ones(64,3,32,32)
#     output = mode(input)
#     print(output.shape)