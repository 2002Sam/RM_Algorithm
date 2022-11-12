# RM_Algorithm
## Armor identification
***
  ### using traditional way to recognize Armor:
  #### 1.img preprocessing:
  using channel substraction to avoid identify the color:

      cv::subtract(channels[2], channels[0], _seperationSrcBlue);
      cv::subtract(channels[0], channels[2], _seperationSrcRed);

      
  use & to mix different pictures

      _maxColor = _seperationSrcRed & _graySrc;
		cv::bitwise_not(_maxColor, _maxColor);
		_maxColor = white_binary & _maxColor;

     
  #### 2.find the lightbar:

 	 cv::findContours(_maxColor, allContours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);//找轮廓

  #### 3.find the proper lightbar:
  request:  long side/shortside is in a range
  area is in a range 
  hight/width is in a range

		if (lightContoursArea < _para.minLightBarArea || lightContoursArea > _para.maxLightBarArea) 
		{/* std::cout << lightContoursArea << std::endl;*/ continue; }
		//cv::RotatedRect lightRect = cv::fitEllipse(contours);
		if (lightRect_par.br().y - lightRect_par.tl().y > (lightRect_par.br().x - lightRect_par.tl().x) * 10.0f ||lightRect_par.br().y - lightRect_par.tl().y < (lightRect_par.br().x - lightRect_par.tl().x) * 1.5f)
				continue;

	
#### 4.conbine the lightbars together:
	size1/size2 is in a range 
	
#### 5.put the lightbar into a structure named armorstructs

        ArmorStruct armorStruct;
	armorStruct.armor = Util::boundArmor(possibleRect[i], possibleRect[j]);
	armorStruct.lightBars[0] = possibleRect[i].center.x - possibleRect[j].center.x > 0 ? possibleRect[j] : possibleRect[i];//左边的bar
	armorStruct.lightBars[1] = possibleRect[i].center.x - possibleRect[j].center.x > 0 ? possibleRect[i] : possibleRect[j];
	armorStruct.barNum1 = possibleRect[i].center.x - possibleRect[j].center.x > 0 ? j : i;
	armorStruct.barNum2 = possibleRect[i].center.x - possibleRect[j].center.x > 0 ? i : j;
	armorStructs.push_back(armorStruct);//填入vector
  


