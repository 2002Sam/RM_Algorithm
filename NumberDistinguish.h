#pragma once
#include <torch/torch.h>
#include <torch/script.h>
#include <iostream>
#include<opencv2/opencv.hpp>
#include"armorDetection.h"

class Detector {
public:
	/***
	 * @brief constructor
	 * @param model_path - path of the TorchScript weight file
	 * @param device_type - inference with CPU/GPU
	 */
	Detector(const std::string& model_path, bool use_gpu);

	// other methond
	int Run(cv::Mat src);



private:
	torch::jit::script::Module module_;
	torch::Device device_;
	bool half_;
	cv::Mat src;
};
