#include"NumberDistinguish.h"

Detector::Detector(const std::string& model_path, bool use_gpu) :device_(torch::kCUDA)
{
	//if (torch::cuda::is_available() && use_gpu)
	//{
	//	//std::cout << "use cuda...\n";
	//	device_ = torch::kCUDA;
	//}
	//else
	//{
	//	//std::cout << "use cpu...\n";
	//	device_ = torch::kCPU;
	//}


//
// 程序开始植入代码
	//int8_t cuda_index = 1;
	//c10::cuda::CUDAGuard device_guard(cuda_index);
	torch::cuda::device_count();
	std::cout << "cudn是否可用：" << torch::cuda::cudnn_is_available() << std::endl;
	std::cout << "device_count" << torch::cuda::device_count()<< std::endl;
	try {

		// Deserialize the ScriptModule from a file using torch::jit::load().
		module_ = torch::jit::load(model_path);
		module_.to(at::kCUDA);

	}
	catch (const c10::Error& e) {
		std::cerr << "Error loading the model!\n";
		std::exit(EXIT_FAILURE);
	}

	//half_ = (device_ != torch::kCPU);
	//module_.to(device_);

	//if (half_) {
	//	module_.to(torch::kHalf);
	//}

	module_.eval();
	

	//cv::waitKey(1000);
}

int Detector::Run(cv::Mat image)
{
	//cv::Mat image;
	//image = cv::imread(ArmorDistinguish::getPath() + "img_1.jpg", cv::IMREAD_GRAYSCALE);//读取灰度图
	cv::resize(image, image, cv::Size(28, 28));
	cv::imshow("dd", image);
	std::vector<int64_t> sizes = { 1, 1, image.rows, image.cols };  //依次为batchsize、通道数、图像高度、图像宽度
	at::TensorOptions options(at::ScalarType::Byte);
	at::Tensor tensor_image = torch::from_blob(image.data, at::IntList(sizes), options);//将opencv的图像数据转为Tensor张量数据
	tensor_image = tensor_image.toType(at::kFloat);//转为浮点型张量数据
	tensor_image = tensor_image.to(at::kCUDA);
	torch::NoGradGuard no_grad;
	at::Tensor result = module_.forward({ tensor_image }).toTensor();//推理
	std::cout << result << std::endl;
	auto max_result = result.max(1, true);
	auto max_index = std::get<1>(max_result).item<float>();
	std::cerr << "检测结果为：";
	std::cout << max_index+1 << std::endl;
	return max_index;
}




