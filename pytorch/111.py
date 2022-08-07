# 完整的一个模板
import torch
import torchvision.datasets
from torch.utils.tensorboard import SummaryWriter

from model import *
#准备数据集
from torch import nn
from torch.utils.data import DataLoader
transform = torchvision.transforms.Compose([torchvision.transforms.ToTensor(),
                                            torchvision.transforms.Normalize(
                                                (0.1307,), (0.3081,))])

#训练数据集
train_data = torchvision.datasets.MNIST(root="../data",train=True,download=True,
                                        transform=transform)
test_data = torchvision.datasets.MNIST(root="../data",train=False,transform=transform)




#数据集大小
train_data_size = len(train_data)
test_data_size = len(test_data)
print("训练集的长度为：{}".format(train_data_size))
print("测试集的长度为：{}".format(test_data_size))

#用dataloader进行加载
train_dataloader = DataLoader(train_data,batch_size=64,shuffle=True)#batchsize:每次load的数量
test_dataloader = DataLoader(test_data,batch_size=64,shuffle=True)

#搭建神经网络：在文件model.py中

#创建网络模型：
mode = Neet()
#使用GPU
mode = mode.cuda()

#创建损失函数：
loss_fn = nn.CrossEntropyLoss()
loss_fn = loss_fn.cuda()

#优化器
learning_rate = 1e-2
optimizer = torch.optim.SGD(mode.parameters(),lr=learning_rate)

# 设置训练网络的参数：
#记录训练的次数：
total_train_step = 0
#记录测试的次数
total_test_step = 0
#训练的轮数
epoch = 10

#添加tensorboard
writer = SummaryWriter("./train")
for i in range(epoch):
    print("--------第{}轮训练----------".format(i+1))

    #训练开始：
    mode.train()
    for batch_idx, (imgs, targets) in enumerate(train_dataloader):
        imgs = imgs.cuda()
        print(imgs.shape)
        targets = targets.cuda()
        print("this is targets")
        print(targets)
        outputs = mode(imgs)
        print("this is outputs")
        print(outputs)
        loss = loss_fn(outputs,targets) #得到损失
        # 优化器优化模型
        optimizer.zero_grad()
        loss.backward()  #反向传播
        optimizer.step() #进行优化
        total_train_step+=1
        if total_test_step % 100 ==0:
            print("训练次数：{}， loss:{}".format(total_train_step,loss.item()))
            writer.add_scalar("train_loss",loss.item(),total_train_step)
    # 测试步骤开始
    mode.eval()
    total_test_lose = 0
    total_currency = 0
    with torch.no_grad():
        for data in test_dataloader:
            imgs,targets = data
            imgs = imgs.cuda()
            targets = targets.cuda()
            print(targets)
            outputs = mode(imgs)
            loss = loss_fn(outputs,targets)
            total_test_lose+=loss.item()
            print(outputs.argmax(1))
            print(targets)
            accuracy = (outputs.argmax(1) == targets).sum()
            total_currency+=accuracy
    print("整体测试集上的Loss:{}".format(total_test_lose))
    print("整体测试集上的正确率：{}".format(total_currency/test_data_size))
    writer.add_scalar("test_loss",total_test_lose,total_test_step)
    writer.add_scalar("测试的正确率",total_currency/test_data_size,total_test_step)
    total_test_step+=1
    torch.save(mode,"mode_{}.pth".format(i))
    #保存方式2：
    # torch.save(mode.state_dict(),"tudui_{}.pth".format(i))
    print("模型已保存")
writer.close()