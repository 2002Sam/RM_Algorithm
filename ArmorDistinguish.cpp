#include"armorDetection.h"

//#define DEBUG
#define USE_GUIDIAN     //比较适用于较暗环境

void ArmorDistinguish::imagePreprogress(const cv::Mat& src, EnemyColor enimycolor)
{
	//ROI
	const cv::Point& lastResult = _resLast.center;
	if (lastResult.x == 0 || lastResult.y == 0)
	{
		_src = src;
		restoreRect = cv::Rect(0, 0, src.rows, src.cols);
	}
	else
	{
		cv::Rect rect = _resLast.boundingRect();
		static float ratioToWidth = 4.8f;
		static float ratioToHeight = 3.0f;
		static float x1 = MAX(lastResult.x - ratioToWidth * rect.width, 0);
		static float y1 = MAX(lastResult.y - ratioToWidth * rect.height, 0);
		cv::Point lt = cv::Point(x1, y1);
		static float x2 = MIN(lastResult.x - ratioToWidth * rect.width, src.cols);
		static float y2 = MIN(lastResult.y - ratioToWidth * rect.height, src.rows);
		cv::Point rb = cv::Point(x2, y2);
		restoreRect = cv::Rect(lt, rb);
		src(restoreRect).copyTo(_src);
	}

	cv::split(_src, channels);
	cv::cvtColor(_src, _graySrc, cv::COLOR_BGR2GRAY);
	cv::threshold(_graySrc, white_binary, 140, 255, cv::THRESH_BINARY);


	if (enimycolor == ENEMY_RED)//敌方为红色
	{

#ifdef USE_GUIDIAN	
		cv::bitwise_not(white_binary, white_binary);   //黑白转换,取得比较白（亮）的部分	
		cv::threshold(channels[2], _seperationPerple, _para.grayThreshold_PURPLE, 255, cv::THRESH_BINARY);
		cv::bitwise_not(_seperationPerple, _seperationPerple);  //对紫色进行区分
		cv::threshold(_graySrc, _graySrc, 180, 255, cv::THRESH_BINARY);//灰度图二值化
		cv::bitwise_not(_graySrc, _graySrc);
		//cv::morphologyEx(_graySrc, _graySrc, cv::MORPH_OPEN, Util::structuringElement3());
		cv::subtract(channels[2], channels[1], _seperationSrcGreen);
		cv::subtract(channels[2], channels[0], _seperationSrcBlue);
		cv::threshold(_seperationSrcGreen, _seperationSrcGreen, _para.separationThreshold_GREEN, 255, cv::THRESH_BINARY);
		cv::morphologyEx(_seperationSrcGreen, _seperationSrcGreen, cv::MORPH_OPEN, Util::structuringElement5());
		cv::threshold(_seperationSrcBlue, _seperationSrcBlue, _para.separationThreshold_RED, 255, cv::THRESH_BINARY);
		cv::morphologyEx(_seperationSrcBlue, _seperationSrcBlue, cv::MORPH_OPEN, Util::structuringElement5());
		cv::dilate(_seperationSrcGreen, _seperationSrcGreen, Util::structuringElement3());
		cv::dilate(_seperationSrcBlue, _seperationSrcBlue, Util::structuringElement3());
		_maxColor = white_binary & _seperationSrcBlue & _graySrc;
		//cv::bitwise_not(_maxColor, _maxColor);
		//cv::dilate(_maxColor, _maxColor, Util::structuringElement3());
		//cv::morphologyEx(_maxColor, _maxColor, cv::MORPH_OPEN, Util::structuringElement5());
		cv::morphologyEx(_maxColor, _maxColor, cv::MORPH_CLOSE, Util::structuringElement3());
		cv::morphologyEx(_maxColor, _maxColor, cv::MORPH_CLOSE, Util::structuringElement3());
		//cv::erode(_maxColor, _maxColor, Util::structuringElement3());
#else
		cv::threshold(channels[2], red, 180, 255, cv::THRESH_BINARY);

#endif
	}
	else   //敌方为蓝色
	{
#ifdef USE_GUIDIAN
		//cv::bitwise_not(white_binary, white_binary);   //黑白转换,取得比较白（亮）的部分
		cv::threshold(channels[2], _seperationPerple, _para.grayThreshold_PURPLE, 255, cv::THRESH_BINARY);
		cv::bitwise_not(_seperationPerple, _seperationPerple);  //对紫色进行区分
		cv::threshold(_graySrc, _graySrc, 100, 255, cv::THRESH_BINARY);
		cv::bitwise_not(_graySrc, _graySrc);
		//cv::morphologyEx(_graySrc, _graySrc, cv::MORPH_OPEN, Util::structuringElement3());
		cv::subtract(channels[0], channels[1], _seperationSrcGreen);
		cv::subtract(channels[0], channels[2], _seperationSrcRed);
		cv::threshold(_seperationSrcGreen, _seperationSrcGreen, _para.separationThreshold_GREEN, 255, cv::THRESH_BINARY);
		cv::morphologyEx(_seperationSrcGreen, _seperationSrcGreen, cv::MORPH_OPEN, Util::structuringElement3());
		cv::threshold(_seperationSrcRed, _seperationSrcRed, _para.separationThreshold_BLUE, 255, cv::THRESH_BINARY);
		cv::morphologyEx(_seperationSrcRed, _seperationSrcRed, cv::MORPH_OPEN, Util::structuringElement3());
		cv::dilate(_seperationSrcGreen, _seperationSrcGreen, Util::structuringElement3());
		cv::dilate(_seperationSrcRed, _seperationSrcRed, Util::structuringElement3());
		//cv::bitwise_not(_seperationSrcRed, _seperationSrcRed);
		_maxColor = _seperationSrcRed & _graySrc;
		cv::bitwise_not(_maxColor, _maxColor);
		_maxColor = white_binary & _maxColor;
		cv::morphologyEx(_maxColor, _maxColor, cv::MORPH_OPEN, Util::structuringElement3());
		cv::morphologyEx(_maxColor, _maxColor, cv::MORPH_OPEN, Util::structuringElement3());
		//cv::erode(_maxColor, _maxColor, Util::structuringElement3());
#else


		cv::threshold(channels[0], blue, 180, 255, cv::THRESH_BINARY);
#endif 

	}



}

ArmorDistinguish::ArmorDistinguish()
{
}

cv::Mat ArmorDistinguish::process(const cv::Mat& src)
{
	//srcBright = changeBright(src);
	imagePreprogress(src, enimyColor);
	//cv::imshow("原图像", src);
	findLightBarContour();
	lightBarFilter();

#ifdef DEBUG
#ifdef USE_GUIDIAN  _seperationSrcRed & _graySrc & white_binary;

	//cv::imshow("blue", channels[0]);
	//cv::imshow("green", channels[1]);
	//cv::imshow("red", channels[2]);
	//cv::imshow("_seperationSrcRed", _seperationSrcRed);
	//cv::imshow("_seperationSrcBlue", _seperationSrcBlue);
	//cv::imshow("_separationSrc", _separationSrc);
	//cv::imshow("_graySrc", _graySrc);
	//cv::imshow("_separationSrcGreen", _separationSrcGreen);
	//cv::imshow("_separationSrcWhite", _separationSrcWhite);
	//cv::imshow("_purpleSrc", _purpleSrc);
	//cv::imshow("white_binary", white_binary);
	//cv::imshow("_maxColor", _maxColor);
#else
	cv::imshow("white_binary", white_binary);
	cv::imshow("src", src);
	cv::imshow("blue", blue);
#endif
	//cv::imshow("src", _src);

#endif // DEBUG
	cv::namedWindow("image", 0);
	cv::imshow("image", _src);
	/*cv::namedWindow("white_binary", 0);
	cv::imshow("white_binary", white_binary);
	cv::namedWindow("_maxColor", 0);
	cv::imshow("_maxColor", _maxColor);
	cv::namedWindow("_seperationSrcRed", 0);
	cv::imshow("_seperationSrcRed", _seperationSrcRed);
	cv::namedWindow("_graySrc", 0);
	cv::imshow("_graySrc", _graySrc);*/
	return _src;
}

void ArmorDistinguish::findLightBarContour()//找发光体轮廓
{
	cv::findContours(_maxColor, allContours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);//找轮廓

	//cv::findContours(white_binary, allContours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);//找轮廓

	for (int i = 0; i < allContours.size(); ++i)
	{

		{
			float lightContoursArea = cv::contourArea(allContours[i]);
			//draw_(_src, lightRect, cv::Scalar(0, 0, 0));
			if (lightContoursArea < _para.minLightBarArea || lightContoursArea > _para.maxLightBarArea) {/* std::cout << lightContoursArea << std::endl;*/ continue; }
			//cv::RotatedRect lightRect = cv::fitEllipse(contours);
			cv::RotatedRect lightRect = cv::minAreaRect(allContours[i]);



			cv::Rect lightRect_par = cv::boundingRect(allContours[i]);
			float longside = Util::findExtreamSide(lightRect, LONG);
			float shortside = Util::findExtreamSide(lightRect, SHORT);
			if (longside > _para.maxLightBarLength || longside < _para.minLightBarLength || shortside > _para.maxLightBarWidth || shortside < _para.minLightBarWidth)
				continue; //busi

#ifdef DEBUG
			//draw_(_src, lightRect, cv::Scalar(0, 255, 0));
#endif // DEBUG



			/*if (longside > shortside * 12.0f || longside < shortside * 1.5f)
				continue;*/

				//draw_(_src, lightRect, cv::Scalar(255, 0, 255));

			if (lightRect_par.br().y - lightRect_par.tl().y > (lightRect_par.br().x - lightRect_par.tl().x) * 10.0f ||
				lightRect_par.br().y - lightRect_par.tl().y < (lightRect_par.br().x - lightRect_par.tl().x) * 1.5f)
				continue;

			possibleRect.push_back(lightRect);
#ifdef DEBUG
			std::cout << possibleRect.size();
			//draw_(_src, lightRect, cv::Scalar(0, 0, 255));
#endif // DEBUG
			//draw_(_src, lightRect, cv::Scalar(0, 0, 255));

		}
	}
}

void ArmorDistinguish::lightBarFilter()
{
	std::vector<ArmorStruct> armorStructs;
	if (possibleRect.size() < 2)
	{
		possibleRect.clear();
		return;
	}
	for (int i = 0; i < possibleRect.size() - 1; i++)//选取合适的灯条进行组合
	{
		for (int j = i + 1; j < possibleRect.size(); j++)
		{
			float maxLength1 = Util::findExtreamSide(possibleRect[i], LONG);
			float minLength1 = Util::findExtreamSide(possibleRect[i], SHORT);
			float maxLength2 = Util::findExtreamSide(possibleRect[j], LONG);
			float minLength2 = Util::findExtreamSide(possibleRect[j], SHORT);
			float maxLengthBoth = MAX(maxLength1, maxLength2);
			float minLengthBoth = MIN(maxLength1, maxLength2);

			if (std::fabs(possibleRect[i].center.y - possibleRect[j].center.y) > minLengthBoth * 0.5f) { continue; }   //判断中点y坐标
			/*draw_(_src, possibleRect[i], cv::Scalar(0, 0, 255));
			draw_(_src, possibleRect[j], cv::Scalar(0, 0, 255));*/
			if (std::fabs(possibleRect[i].center.x - possibleRect[j].center.x) < 1.5*maxLengthBoth || std::fabs(possibleRect[i].center.x - possibleRect[j].center.x) > minLengthBoth * 3.5)        //判断最长边和两条距离
				continue;
			
			if (maxLength1 / maxLength2 > 1.5 || maxLength1 / maxLength2 < 0.75)//判断两bar长之比
				continue;

			cv::Point2f point1[4];
			cv::Point2f point2[4];
			possibleRect[i].points(point1);
			possibleRect[j].points(point2);
#ifdef DEBUG
			std::cout << "point1" << point1[0] << "-----" << point1[1] << "-----" << point1[2] << "-----" << point1[3] << std::endl;
			std::cout << "point2" << point2[0] << "-----" << point2[1] << "-----" << point2[2] << "-----" << point2[3] << std::endl;
#endif // DEBUG

			cv::Point2f shortcenterts1[2];
			cv::Point2f shortcenterts2[2];
			getShortCenter(possibleRect[i], shortcenterts1);
			getShortCenter(possibleRect[j], shortcenterts2);
#ifdef DEBUG
			std::cout << "shortcenter1" << shortcenterts1[0] << "----" << shortcenterts2[1] << "shortcenter2" << shortcenterts2[0] << shortcenterts2[1] << std::endl;
			std::cout << Util::findLineandLineAngle(shortcenterts1[0], shortcenterts1[1], shortcenterts2[0], shortcenterts2[1]) << std::endl;
#endif // DEBUG
			if (Util::findLineandLineAngle(shortcenterts1[0], shortcenterts1[1], shortcenterts2[0], shortcenterts2[1]) > 7.0f)
				continue;
			//判断两线夹角


			if (possibleRect[i].size.area() / possibleRect[j].size.area() > 7 || possibleRect[i].size.area() / possibleRect[j].size.area() < 0.2)//判断面积
				continue;

			

			ArmorStruct armorStruct;
			armorStruct.armor = Util::boundArmor(possibleRect[i], possibleRect[j]);
			armorStruct.lightBars[0] = possibleRect[i].center.x - possibleRect[j].center.x > 0 ? possibleRect[j] : possibleRect[i];//左边的bar
			armorStruct.lightBars[1] = possibleRect[i].center.x - possibleRect[j].center.x > 0 ? possibleRect[i] : possibleRect[j];
			armorStruct.barNum1 = possibleRect[i].center.x - possibleRect[j].center.x > 0 ? j : i;
			armorStruct.barNum2 = possibleRect[i].center.x - possibleRect[j].center.x > 0 ? i : j;

			armorStructs.push_back(armorStruct);//填入vector

			

		}
	}
	possibleRect.clear();


	if (armorStructs.size() > 1)
	{
		for (int i = 0; i < armorStructs.size() - 1; i++)
		{
			for (int j = i + 1; j < armorStructs.size(); j++)
			{
				if (armorStructs[i].barNum1 == armorStructs[j].barNum1)//如果左侧灯条相同
				{
					if (armorStructs[i].lightBars[1].center.x < armorStructs[j].lightBars[1].center.x)
						armorStructs.erase(std::begin(armorStructs) + j);
					else
						armorStructs.erase(std::begin(armorStructs) + i);
				}
				if (armorStructs[i].barNum2 == armorStructs[j].barNum2)//如果右侧灯条相同
				{
					if (armorStructs[i].lightBars[0].center.x > armorStructs[0].lightBars[1].center.x)
						armorStructs.erase(std::begin(armorStructs) + j);
					else
						armorStructs.erase(std::begin(armorStructs) + i);
				}
			}
		}
	}
	if (armorStructs.size() > 0)
	{
		for (int i = 0; i < armorStructs.size(); i++)
		{
			draw_(_src, armorStructs[i].lightBars[0], cv::Scalar(0, 255, 255));
			draw_(_src, armorStructs[i].lightBars[1], cv::Scalar(0, 255, 255));
			draw_(_src, armorStructs[i].armor, cv::Scalar(255, 255, 0));
		}
	}

	armorStructs.clear();
	allContours.clear();
	hierarchy.clear();

}

void ArmorDistinguish::getShortCenter(cv::RotatedRect& rect, cv::Point2f* shortcenterts)
{
	cv::Point2f point[4];
	rect.points(point);
	shortcenterts[0] = Util::pointToPointDistance(point[0], point[1]) > Util::pointToPointDistance(point[1], point[2]) ? (point[1] + point[2]) / 2 : (point[0] + point[1]) / 2;
	shortcenterts[1] = Util::pointToPointDistance(point[2], point[3]) > Util::pointToPointDistance(point[0], point[3]) ? (point[0] + point[3]) / 2 : (point[2] + point[3]) / 2;

}

void ArmorDistinguish::ifbarInside(ArmorStruct& a1, ArmorStruct& a2)
{

}

void ArmorDistinguish::draw_(cv::Mat src, cv::RotatedRect rect, cv::Scalar ss) {
	cv::Point2f P[400];
	rect.points(P);
	for (int j = 0; j <= 3; j++)
	{
		{
			cv::line(src, P[j], P[(j + 1) % 4], ss, 2);
		}

	}
}

cv::Mat ArmorDistinguish::changeBright(const cv::Mat& src)
{
	cv::Mat dst1, dst2, dst3;
	dst1 = cv::Mat::zeros(src.size(), src.type());
	double alpha = 0.9;
	double beta = -10.0f;
	double gama = 2.0f;

	// 提示并输入 α  β  γ  的值
	/*cout << " Basic Linear Transforms " << endl;
	cout << "-------------------------" << endl;
	cout << "* Enter the alpha value [1.0-3.0]: "; cin >> alpha;
	cout << "* Enter the beta value [0-100]: ";    cin >> beta;
	cout << "* Enter the gama value [-1,1]: ";    cin >> gama;*/

	// 直接使用循环遍历每一个像素，应用公式
	double t1 = (double)cv::getTickCount();
	for (int row = 0; row < src.rows; ++row)
		for (int col = 0; col < src.cols; ++col)
			for (int channel = 0; channel < src.channels(); ++channel)
			{
				dst1.at<cv::Vec3b>(row, col)[channel] = cv::saturate_cast<uchar>(alpha * src.at<cv::Vec3b>(row, col)[channel] + beta);
			}
	double time1 = ((double)cv::getTickCount() - t1) / cv::getTickFrequency();
	//std::cout << "Method by pixel use time:" << time1 << "(ms)" << std::endl;

	// 调用 convertTo() 函数调整对比度和亮度
	double t2 = (double)cv::getTickCount();
	src.convertTo(dst2, -1, alpha, beta);
	double time2 = ((double)cv::getTickCount() - t2) / cv::getTickFrequency();
	//std::cout << "Method by pixel use time:" << time2 << "(ms)" << std::endl;

	// 构建查找表
	cv::Mat lookUpTable(1, 256, CV_8U);
	uchar* p = lookUpTable.ptr();
	for (int i = 0; i < 256; ++i)
		p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gama) * 255.0);

	// 使用查找表进行对比度亮度调整
	double t3 = (double)cv::getTickCount();
	LUT(src, lookUpTable, dst3);
	double time3 = ((double)cv::getTickCount() - t3) / cv::getTickFrequency();
	//std::cout << "Method by γ correct use time:" << time3 << "(ms)" << std::endl;
#ifdef DEBUG
	// 调整窗体大小，显示调整效果
	/*cv::namedWindow("original", cv::WINDOW_NORMAL);
	cv::resizeWindow("original", cv::Size(src.cols / 2, src.rows / 2));
	cv::imshow("original", src);
	cv::namedWindow("pixel set", cv::WINDOW_NORMAL);
	cv::resizeWindow("pixel set", cv::Size(src.cols / 2, src.rows / 2));
	cv::imshow("pixel set", dst1);
	cv::namedWindow("convertTo", cv::WINDOW_NORMAL);
	cv::resizeWindow("convertTo", cv::Size(src.cols / 2, src.rows / 2));
	cv::imshow("convertTo", dst2);
	cv::namedWindow("γ correct", cv::WINDOW_NORMAL);
	cv::resizeWindow("γ correct", cv::Size(src.cols / 2, src.rows / 2));
	cv::imshow("γ correct", dst3);*/

#endif
	return dst3;

}






