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
// ����ʼֲ�����
	//int8_t cuda_index = 1;
	//c10::cuda::CUDAGuard device_guard(cuda_index);
	torch::cuda::device_count();
	std::cout << "cudn�Ƿ���ã�" << torch::cuda::cudnn_is_available() << std::endl;
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
	//image = cv::imread(ArmorDistinguish::getPath() + "img_1.jpg", cv::IMREAD_GRAYSCALE);//��ȡ�Ҷ�ͼ
	cv::resize(image, image, cv::Size(28, 28));
	cv::imshow("dd", image);
	std::vector<int64_t> sizes = { 1, 1, image.rows, image.cols };  //����Ϊbatchsize��ͨ������ͼ��߶ȡ�ͼ����
	at::TensorOptions options(at::ScalarType::Byte);
	at::Tensor tensor_image = torch::from_blob(image.data, at::IntList(sizes), options);//��opencv��ͼ������תΪTensor��������
	tensor_image = tensor_image.toType(at::kFloat);//תΪ��������������
	tensor_image = tensor_image.to(at::kCUDA);
	torch::NoGradGuard no_grad;
	at::Tensor result = module_.forward({ tensor_image }).toTensor();//����
	std::cout << result << std::endl;
	auto max_result = result.max(1, true);
	auto max_index = std::get<1>(max_result).item<float>();
	std::cerr << "�����Ϊ��";
	std::cout << max_index+1 << std::endl;
	return max_index;
}




