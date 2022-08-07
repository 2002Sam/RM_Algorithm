#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <torch/torch.h>
#include <torch/script.h>
#include"armorDetection.h"
// 有人说调用的顺序有关系，我这好像没啥用~~
torch::Tensor ToTensor(cv::Mat& image)
{
    torch::Tensor img_tensor = torch::from_blob(image.data, { image.rows, image.cols, 3 }, torch::kByte);
    img_tensor = img_tensor.permute({ 2, 0, 1 }).toType(torch::kFloat).div(255);
    img_tensor = torch::data::transforms::Normalize<torch::Tensor>({ 0.485, 0.456, 0.406 }, { 0.229, 0.224, 0.225 })(img_tensor);
    img_tensor = img_tensor.unsqueeze(0);
    return img_tensor;
}
//int main()
//{
//    torch::DeviceType device_type;
//    if (torch::cuda::is_available()) {
//        std::cout << "CUDA available! Predicting on GPU." << std::endl;
//        device_type = torch::kCUDA;
//    }
//    else {
//        std::cout << "Predicting on CPU." << std::endl;
//        device_type = torch::kCPU;
//    }
//    torch::Device device(device_type);
//
//    //Init model
//    std::string model_pb = "C:/Users/86133/Desktop/mode_9.pt";
//    auto module = torch::jit::load(model_pb);
//    module.to(at::kCUDA);
//
//    cv::Mat image = cv::imread(ArmorDistinguish::getPath() + "img_1.png");
//    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
//    cv::resize(image, image, cv::Size(28, 28));
//
//    // convert to tensort
//
//    torch::Tensor img_tensor = ToTensor(image).to(at::kCUDA);  //ToTensor是自定义函数，下面有
//
//    torch::Tensor output = module.forward({ image }).toTensor();
//    auto max_result = output.max(1, true);
//    auto max_index = std::get<1>(max_result).item<float>();  //找出最大值的索引
//
//    return max_index;
//
//}
