# RM_Algorithm
## Armor identification
***
  ### using traditional way to recognize Armor:
  #### 1.img preprocessing 
      using channel substraction to avoid identify the color:
      ```C++
      cv::subtract(channels[2], channels[0], _seperationSrcBlue);
      cv::subtract(channels[0], channels[2], _seperationSrcRed);
      ```
      use & to mix different pictures
       ```C++
      _maxColor = _seperationSrcRed & _graySrc;
		cv::bitwise_not(_maxColor, _maxColor);
		_maxColor = white_binary & _maxColor;
     ```
     
  #### 2.find the lightbar:
  ```C++
  cv::findContours(_maxColor, allContours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);//找轮廓
  ```
  #### 3.find the proper lightbar:
  reuest: >long side/shortside is in a range
          
  

