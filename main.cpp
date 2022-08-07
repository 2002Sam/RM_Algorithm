#include"armorDetection.h"
ArmorDistinguish armorDistinguish;
//#define PIC
#define VIDEO
int main()
{
#ifdef PIC
	cv::Mat src = cv::imread(ArmorDistinguish::getPath() + "1.jpg", 1);
	armorDistinguish.process(src);
	cv::waitKey(0);
#endif

#ifdef VIDEO
	cv::VideoCapture cap(ArmorDistinguish::getPath() + "2022-04-17 03-51-53.mkv");
	int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);             //֡���
	int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);           //֡�߶�
	int totalFrames = cap.get(cv::CAP_PROP_FRAME_COUNT);       //��֡��
	double frameRate = cap.get(cv::CAP_PROP_FPS);                 //֡�� x frames/s
	std::cout << "֡�ʣ� " << frameRate << std::endl;
	cv::Mat frame;
	cv::VideoWriter writer;
	writer.open(ArmorDistinguish::getPath()+"ans.avi", cv::VideoWriter::fourcc('M','J','P','G'), frameRate, cv::Size(width,height),true);
	if (cap.isOpened())
	{
		while (cap.grab()) 
		{

			cap >> frame;
			cv::Mat src = armorDistinguish.process(frame);
			if (frame.empty())break;
			writer << src;
			cv::waitKey(30);
			/*imshow("images", src);
			cv::waitKey(10);*/
		}
	}
	cv::waitKey();
#endif
}


	//std::shared_ptr<Detector> detector = std::make_shared<Detector>("C:/Users/86133/Desktop/mode_9.pt", true);//����ָ��

