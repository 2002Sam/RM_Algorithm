#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include<string>
#include<algorithm>
#include<math.h>
#include<cmath>
#include"RMDefine.h"
#include<vector>
#include"Conf.h"
#include"PointUtil.h"


class ArmorDistinguish
{
private:
	static ArmorDistinguish& instance()
	{
		static ArmorDistinguish armorDistinguish;
		return armorDistinguish;
	}
	struct ArmorStruct
	{
		cv::RotatedRect lightBars[2];
		cv::RotatedRect armor;
		float lightAngle;
		int carNum = 0;
		int barNum1 = 0;
		int barNum2 = 0;
	};
	const std::string path = "C:/Users/86133/Desktop/zhuanjiaban/";
	cv::RotatedRect _resLast;//��һ֡�ļ������
	cv::Mat srcBright;
	cv::Mat _src;
	cv::Mat blue;
	cv::Mat red;
	std::vector<cv::Mat> splitSrc;
	cv::Rect restoreRect;
	std::vector<cv::Mat> channels;
	cv::Mat _graySrc;
	cv::Mat white_binary;
	cv::Mat _seperationSrcGreen;
	cv::Mat _seperationSrcBlue;
	cv::Mat _seperationSrcRed;
	cv::Mat _maxColor;
	cv::Mat _seperationPerple;
	cv::Mat _purpleSrc;
	cv::Mat _separationSrc;
	cv::Mat _separationSrcGreen;
	cv::Mat _separationSrcWhite;
	ArmorPara _para;
	EnemyColor enimyColor = ENEMY_BLUE;
	std::vector<std::vector<cv::Point2i>> allContours;
	std::vector<cv::Vec4i> hierarchy;
	std::vector<cv::RotatedRect> possibleRect;

	cv::Mat _seperationSrcGreen__;
public:
	ArmorDistinguish();
	~ArmorDistinguish() {}
	cv::Mat process(const cv::Mat& src);
	static std::string getPath() { return instance().path; }
	void draw_(cv::Mat src, cv::RotatedRect rect, cv::Scalar ss);
    cv::Mat changeBright(const cv::Mat &src);
private:
	//��ͼ�����Ԥ����,ROI�Ͷ�ֵ��
	void imagePreprogress(const cv::Mat &src,EnemyColor enimycolor);
	
	//�Ե������й���
	void findLightBarContour();

	//������ƥ��ĵ������а�
	void lightBarFilter();

	void getShortCenter(cv::RotatedRect& rect, cv::Point2f* shortcenterts);

	void ifbarInside(ArmorStruct &a1, ArmorStruct &a2);//�ж��Ƿ��й��õ���������ȥ��
};

//cv::bitwise_not(white_binary, white_binary);   //�ڰ�ת��,ȡ�ñȽϰף������Ĳ���	
		//cv::threshold(_graySrc, _graySrc, _para.grayThreshold_RED, 255, cv::THRESH_BINARY);//�Ҷ�ͼ��ֵ��
		//cv::subtract(channels[2], channels[1], _seperationSrcGreen);
		//cv::subtract(channels[2], channels[0], _seperationSrcBlue);
		//cv::threshold(_seperationSrcGreen, _seperationSrcGreen, _para.separationThreshold_GREEN, 255, cv::THRESH_BINARY);
		//cv::morphologyEx(_seperationSrcGreen, _seperationSrcGreen, cv::MORPH_OPEN, Util::structuringElement5());
		//cv::threshold(_seperationSrcBlue, _seperationSrcBlue, _para.separationThreshold_RED, 255, cv::THRESH_BINARY);
		//cv::morphologyEx(_seperationSrcBlue, _seperationSrcBlue, cv::MORPH_OPEN, Util::structuringElement5());
		//cv::dilate(_seperationSrcGreen, _seperationSrcGreen, Util::structuringElement3());
		//cv::dilate(_seperationSrcBlue, _seperationSrcBlue, Util::structuringElement3());
		//_maxColor = white_binary & _seperationSrcGreen & _seperationSrcBlue & _graySrc;
		//cv::bitwise_not(_maxColor, _maxColor);
		//cv::dilate(_maxColor, _maxColor, Util::structuringElement3());
		//cv::morphologyEx(_maxColor, _maxColor, cv::MORPH_OPEN, Util::structuringElement5());