from PIL import Image
import torch
import xml.etree.ElementTree as ET
import os, cv2
import torch.utils.data as Data
import torchvision
import tensorboard as tensor
from torch.utils.data import DataLoader
from torch.utils.tensorboard import SummaryWriter
import torchvision.datasets
from torch.utils.tensorboard import SummaryWriter
from model import *
#准备数据集
from torch import nn
from torch.utils.data import DataLoader
import os
os.environ["CUDA_VISIBLE_DEVICES"] = "1"

torch.cuda.set_device('cuda:1')

class MyDataset(torch.utils.data.Dataset):  # 创建自己的类：MyDataset,这个类是继承的torch.utils.data.Dataset
    def __init__(self, folder_root,img_folder):  # 初始化一些需要传入的参数
        self.xml_folder = folder_root
        self.img_folder = img_folder
        self.lab_lists = os.listdir(self.xml_folder)
    def __getitem__(self, index):  # 这个方法是必须要有的，用于按照索引读取每个元素的具体内容
        # for i in range(1, len(self.lab_lists)):
        pre_name = self.lab_lists[index].rsplit('.')[0]
        xml_file = self.xml_folder + self.lab_lists[index]
        img_file = self.img_folder + pre_name + ".jpg"
        self.lab_list = self.lab_lists[index]
        num = self.lab_list.split('_')[0]
        # print(num)
        # print(xml_file)
        # print(img_file + '\n')
        tree = ET.parse(xml_file)
        root = tree.getroot()
        im = cv2.imread(img_file)
        for object in root.findall('object'):
            object_name = object.find('name').text
            Xmin = int(object.find('bndbox').find('xmin').text)
            Ymin = int(object.find('bndbox').find('ymin').text)
            Xmax = int(object.find('bndbox').find('xmax').text)
            Ymax = int(object.find('bndbox').find('ymax').text)
            color = (4, 250, 7)
            cv2.rectangle(im, (Xmin, Ymin), (Xmax, Ymax), color, 2)
            num_pic = im[Ymin:Ymax, Xmin:Xmax]
            gray = cv2.cvtColor(num_pic, cv2.COLOR_BGR2GRAY)
            required = cv2.resize(gray, (28, 28), interpolation=cv2.INTER_AREA)
            # trans1 = torchvision.transforms.ToTensor()
            # tran_req = trans1(required)
            # # img = torch.from_numpy(required)
            # tran_req = tran_req.float().div(255).unsqueeze(0)  # 255也可以改为256
            img = torch.from_numpy(required)
            img = img.unsqueeze(0)
            img = img.float()
        # print(img.shape)
        # trans1 = torchvision.transforms.ToTensor()
        # num = trans1(num)
            num = torch.as_tensor(int(object_name))
            return img,num
    def __len__(self):  # 这个函数也必须要写，它返回的是数据集的长度，也就是多少张图片，要和loader的长度作区分
        return len(self.lab_lists)


class MyDatasetTest(torch.utils.data.Dataset):  # 创建自己的类：MyDataset,这个类是继承的torch.utils.data.Dataset
    def __init__(self, folder_root,img_folder):  # 初始化一些需要传入的参数
        self.xml_folder = folder_root
        self.img_folder = img_folder
        self.lab_lists = os.listdir(self.xml_folder)
        self.img_lists = os.listdir(self.img_folder)
    def __getitem__(self, index):  # 这个方法是必须要有的，用于按照索引读取每个元素的具体内容
        # for i in range(1, len(self.lab_lists)):
        xml_file = self.xml_folder + self.lab_lists[index]
        img_file = self.img_folder + self.img_lists[index]
        self.lab_list = self.lab_lists[index]
        self.num = self.lab_list.split('_')[0]
        # print(xml_file)
        # print(img_file + '\n')
        tree = ET.parse(xml_file)
        root = tree.getroot()
        im = cv2.imread(img_file)
        for object in root.findall('object'):
            object_name = object.find('name').text
            Xmin = int(object.find('bndbox').find('xmin').text)
            Ymin = int(object.find('bndbox').find('ymin').text)
            Xmax = int(object.find('bndbox').find('xmax').text)
            Ymax = int(object.find('bndbox').find('ymax').text)
            color = (4, 250, 7)
            cv2.rectangle(im, (Xmin, Ymin), (Xmax, Ymax), color, 2)
            num_pic = im[Ymin:Ymax, Xmin:Xmax]
            cv2.imshow("im",im)
            cv2.waitKey(0)
            gray = cv2.cvtColor(num_pic, cv2.COLOR_BGR2GRAY)
            img = torch.from_numpy(gray)
            img = img.unsqueeze(0)
            # required = cv2.resize(gray, (28, 28), interpolation=cv2.INTER_AREA)
            # img = torch.from_numpy(required.transpose((0,3, 1,2)))
            # tran_req = required.float().div(255).unsqueeze(0)  # 255也可以改为256
            # print(tran_req)

            # tran_req = trans1(required)
            # print(tran_req)
            # print(1)
            # font = cv2.FONT_HERSHEY_SIMPLEX
            # cv2.putText(im, object_name, (Xmin, Ymin - 7), font, 0.5, (6, 230, 230), 2)
            # cv2.namedWindow('01', 0)
            trans1 = torchvision.transforms.ToTensor()
            object_name = torch.as_tensor(int(object_name))
            return img,object_name
    def __len__(self):  # 这个函数也必须要写，它返回的是数据集的长度，也就是多少张图片，要和loader的长度作区分
        return len(self.lab_lists)
# 根据自己定义的那个勒MyDataset来创建数据集！注意是数据集！而不是loader迭代器
# train_data = MyDataset("H:/rm_train_data/self_data/checked/new/annotations/","H:/rm_train_data/self_data/checked/new/img/")
# test_data = MyDatasetTest("H:/rm_train_data/test/number_label/","H:/rm_train_data/test/RMnum/")
train_data = MyDataset("H:/rm_train_data/test/number_label/","H:/rm_train_data/test/RMnum/")
test_data = MyDataset("H:/rm_train_data/test/number_label/","H:/rm_train_data/test/RMnum/")
print("this is train data")
# print(train_data)
train_loader = DataLoader(dataset=train_data, batch_size=64, shuffle=True)
test_dataloader = DataLoader(dataset=test_data,batch_size=64,shuffle=True)


train_data_size = len(train_data)
test_data_size = len(test_data)

train_features, train_labels = next(iter(train_loader))
print(f"Feature batch shape: {train_features.size()}")
# print(f"Labels batch shape: {train_labels.size()}")
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
    for train in train_loader:
        imgs,targets = train
        targets-=1
        imgs = imgs.cuda()
        # print("imgs")
        # print(imgs)
        targets = targets.cuda()
        # outputs = mode(imgs.to(torch.float32))
        outputs = mode(imgs)
        print("outputs")
        print(outputs)
        print("targets")
        print(targets)
        loss = loss_fn(outputs,targets) #得到损失
        # 优化器优化模型
        optimizer.zero_grad()
        loss.backward()  #反向传播
        optimizer.step() #进行优化
        total_train_step+=1
        if total_train_step % 100 ==0:
            print("训练次数：{}， loss:{}".format(total_train_step,loss.item()))
            writer.add_scalar("train_loss",loss.item(),total_train_step)
    # 测试步骤开始
    mode.eval()
    total_test_lose = 0
    total_currency = 0
    with torch.no_grad():
        for data in test_dataloader:
            imgs,targets = data
            targets-=1
            imgs = imgs.cuda()
            targets = targets.cuda()
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

