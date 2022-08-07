if (enemyColor == ENEMY_RED) {
    //�з�Ϊ��ɫ
    cv::threshold(_graySrc, _graySrc, _para.grayThreshold_RED, 255, cv::THRESH_BINARY);     //�Ҷȶ�ֵ��
    cv::subtract(splitSrc[2], splitSrc[0], _separationSrc);

    //����ͨ�����
    cv::subtract(splitSrc[2], splitSrc[1], _separationSrcGreen);                             //����ͨ�����
    cv::threshold(_separationSrc, _separationSrc, _para.separationThreshold_RED, 255, cv::THRESH_BINARY);             //������ֵ��
    cv::threshold(_separationSrcGreen, _separationSrcGreen, _para.separationThreshold_GREEN, 255, cv::THRESH_BINARY);//���̶�ֵ��
    cv::dilate(_separationSrc, _separationSrc, Util::structuringElement3());
    cv::dilate(_separationSrcGreen, _separationSrcGreen, Util::structuringElement3());                                        //����

    _maxColor = _separationSrc & _graySrc & _separationSrcGreen & _separationSrcWhite;                                                                //�߼��������ն�ֵ��ͼ��
    cv::dilate(_maxColor, _maxColor, Util::structuringElement3());                                                    //����
    //cv::morphologyEx(_maxColor, _maxColor, cv::MORPH_OPEN, Util::structuringElement3());
}
else {
    cv::threshold(splitSrc[2], _purpleSrc, _para.grayThreshold_PURPLE, 255, cv::THRESH_BINARY);                 //��ֹ��ʶ����ɫ����
    cv::bitwise_not(_purpleSrc, _purpleSrc);
    //�з�Ϊ��ɫ
    cv::threshold(_graySrc, _graySrc, _para.grayThreshold_BLUE, 255, cv::THRESH_BINARY);                        //�Ҷȶ�ֵ��
    cv::subtract(splitSrc[0], splitSrc[2], _separationSrc);
    cv::subtract(splitSrc[0], splitSrc[1], _separationSrcGreen);                                                //����ͨ�����
    cv::threshold(_separationSrc, _separationSrc, _para.separationThreshold_BLUE, 255, cv::THRESH_BINARY);
    cv::threshold(_separationSrcGreen, _separationSrcGreen, _para.separationThreshold_GREEN, 255, cv::THRESH_BINARY);//������ֵ��
    cv::dilate(_separationSrc, _separationSrc, Util::structuringElement3());
    cv::dilate(_separationSrcGreen, _separationSrcGreen, Util::structuringElement3());           //����

    _maxColor = _separationSrc & _graySrc & _separationSrcGreen & _separationSrcWhite & _purpleSrc;                                                                //�߼��������ն�ֵ��ͼ��
    cv::dilate(_maxColor, _maxColor, Util::structuringElement3());                                                    //����
    //cv::morphologyEx(_maxColor, _maxColor, cv::MORPH_OPEN, Util::structuringElement3());
}



	cv::split(_src, channels);
	cv::cvtColor(_src, _graySrc, cv::COLOR_BGR2GRAY);
	cv::threshold(_graySrc, white_binary, 240, 255, cv::THRESH_BINARY);
	



///////////////////////////////////////////////////////////////////
if (enimycolor == ENEMY_RED)//�з�Ϊ��ɫ
{

#ifdef USE_GUIDIAN	
	cv::bitwise_not(white_binary, white_binary);   //�ڰ�ת��,ȡ�ñȽϰף������Ĳ���	
	cv::threshold(channels[2], _seperationPerple, _para.grayThreshold_PURPLE, 255, cv::THRESH_BINARY);
	cv::bitwise_not(_seperationPerple, _seperationPerple);  //����ɫ��������
	cv::threshold(_graySrc, _graySrc, 180, 255, cv::THRESH_BINARY);//�Ҷ�ͼ��ֵ��
	cv::subtract(channels[2], channels[1], _seperationSrcGreen);
	cv::subtract(channels[2], channels[0], _seperationSrcBlue);
	cv::threshold(_seperationSrcGreen, _seperationSrcGreen, _para.separationThreshold_GREEN, 255, cv::THRESH_BINARY);
	cv::morphologyEx(_seperationSrcGreen, _seperationSrcGreen, cv::MORPH_OPEN, Util::structuringElement5());
	cv::threshold(_seperationSrcBlue, _seperationSrcBlue, _para.separationThreshold_RED, 255, cv::THRESH_BINARY);
	cv::morphologyEx(_seperationSrcBlue, _seperationSrcBlue, cv::MORPH_OPEN, Util::structuringElement5());
	cv::dilate(_seperationSrcGreen, _seperationSrcGreen, Util::structuringElement3());
	cv::dilate(_seperationSrcBlue, _seperationSrcBlue, Util::structuringElement3());
	_maxColor = white_binary/* & _seperationSrcGreen*/ & _seperationSrcBlue & _graySrc;
	//cv::bitwise_not(_maxColor, _maxColor);
	//cv::dilate(_maxColor, _maxColor, Util::structuringElement3());
	//cv::morphologyEx(_maxColor, _maxColor, cv::MORPH_OPEN, Util::structuringElement5());
	cv::morphologyEx(_maxColor, _maxColor, cv::MORPH_CLOSE, Util::structuringElement3());
	cv::erode(_maxColor, _maxColor, Util::structuringElement3());
#else
	cv::threshold(channels[2], red, 180, 255, cv::THRESH_BINARY);

#endif
}
else   //�з�Ϊ��ɫ
{
#ifdef USE_GUIDIAN
	cv::bitwise_not(white_binary, white_binary);   //�ڰ�ת��,ȡ�ñȽϰף������Ĳ���
	cv::threshold(channels[2], _seperationPerple, _para.grayThreshold_PURPLE, 255, cv::THRESH_BINARY);
	cv::bitwise_not(_seperationPerple, _seperationPerple);  //����ɫ��������
	cv::threshold(_graySrc, _graySrc, 80, 255, cv::THRESH_BINARY);
	cv::bitwise_not(_graySrc, _graySrc);
	cv::subtract(channels[0], channels[1], _seperationSrcGreen);
	cv::subtract(channels[0], channels[2], _seperationSrcRed);
	cv::threshold(_seperationSrcGreen, _seperationSrcGreen, _para.separationThreshold_GREEN, 255, cv::THRESH_BINARY);
	cv::morphologyEx(_seperationSrcGreen, _seperationSrcGreen, cv::MORPH_OPEN, Util::structuringElement3());
	cv::threshold(_seperationSrcRed, _seperationSrcRed, _para.separationThreshold_BLUE, 255, cv::THRESH_BINARY);
	cv::morphologyEx(_seperationSrcRed, _seperationSrcRed, cv::MORPH_OPEN, Util::structuringElement3());
	cv::dilate(_seperationSrcGreen, _seperationSrcGreen, Util::structuringElement3());
	cv::dilate(_seperationSrcRed, _seperationSrcRed, Util::structuringElement3());

	_maxColor = _seperationSrcRed & _graySrc & white_binary;


	cv::morphologyEx(_maxColor, _maxColor, cv::MORPH_CLOSE, Util::structuringElement3());
	cv::erode(_maxColor, _maxColor, Util::structuringElement3());
#else


	cv::threshold(channels[0], blue, 180, 255, cv::THRESH_BINARY);
#endif 

}