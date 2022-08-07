#把.pth文件转换为.pt文件
import torch
import torch.nn as nn
import torch.nn.functional as F
import torchvision.models as models
from torchsummary import summary
print(torch.cuda.is_available())
model = torch.load("./mode_9.pth")
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
summary(model, input_size=(1, 28, 28))
model = model.to(device)
traced_script_module = torch.jit.trace(model, torch.ones(1, 1, 28, 28).to(device))
traced_script_module.save("./mode_9.pt")
