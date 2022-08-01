#include"armorDetection.hpp"









int main(int argc, char** argv) {
	Mat src = imread(path+"blue1.jpg", 1); //读取图片
	if (src.empty()) {                     //判断是否找到图片
		printf("没有找到图片 ");           //输出文字
		return -1;
	}
	else
	{
		vector<Mat> channels;
		split(src, channels);
		Mat img_b = channels.at(0);
//		Mat img_g = channels.at(1);
		Mat img_r = channels.at(2);			//分通道

		threshold(img_b, binary, kThreshold, kmax, 1);	//二值化
		GaussianBlur(binary,gauss,gsize,0);
		kernel = getStructuringElement(MORPH_DILATE,Size(3, 3));
		//dilate(gauss, dil, kernel);
		findContours(gauss, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
		vector<RotatedRect> rect_1(contours.size());
		vector<Rect> rect(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			int area = contourArea(contours[i]);
			cout << area << endl;
			if (area > 50 && area < 60000)
			{
				//直接画
				float peri = arcLength(contours[i], true);
				vector<vector<Point>> conPoly(contours.size());				
				approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
				rect[i] = boundingRect(conPoly[i]);
				if (2 * (rect[i].br().x - rect[i].tl().x) < (rect[i].br().y-rect[i].tl().y))
				{
					//rectangle(src, rect[i].tl(), rect[i].br(), Scalar(0, 255, 0), 2);
				}

				
				//if (2 * rect[i].size.height < rect[i].size.width)
				//绘制轮廓的最小外结矩形  
				rect_1[i] = minAreaRect(conPoly[i]);
				//rectangle(画布,rect.boundingRect(),Scalar(55));
				draw_(src, rect_1[i],Scalar(0,0,255));
			}

		}

		
		

		//sort(rect.begin(), rect.end(), func2);

		//sort(rect_1.begin(), rect_1.end(),funcu);
		
		for (int i = 0; i < contours.size(); i++)
		{
			for (int j = i+1; j < contours.size(); j++) {
				

				Point2f P1[3];
				Point2f P2[3];
				(rect_1[i]).points(P1);
				(rect_1[j]).points(P2);
				Point2f a;










				a = P1[0] - P1[1];
				Point2f b;
				b = P1[0] - P1[1];
			

				float cos = (a.x * b.x + a.y * b.y) / (sqrt(pow(a.x, 2) + pow(a.y, 2)) * sqrt(pow(b.x, 2) + pow(b.y, 2)));
				if (
					(fabs(cos) < 0.1) || (fabs(cos) < 1.1 && fabs(cos) > 0.9)

					)
				{










					if (

						(2 * (rect[i].br().x - rect[i].tl().x) < (rect[i].br().y - rect[i].tl().y)) //判断竖矩形长宽比是否大于2
						&& ((2 * (rect[i + 1].br().x - rect[i + 1].tl().x) < (rect[i + 1].br().y - rect[i + 1].tl().y)) || (2 * (rect[i + 1].br().y - rect[i + 1].tl().y) < (rect[i + 1].br().x - rect[i + 1].tl().x)))

						//&&
						//abs(rect_1[i].angle / rect_1[j].angle) > 0.95 && abs(rect_1[i].angle / rect_1[j].angle) < 1.05   //判断斜矩形是否平行

						&&


						 (rect_1[i].center.y - rect_1[j].center.y < 10 && rect_1[i].center.y - rect_1[j].center.y>-10)//判断斜矩形中心y是否接近

					  //&& rect_1[i].size.width
						)//判断竖的rect宽高比
					{

						//draw_(src, rect_1[i], Scalar(0, 0, 0));
						//draw_(src, rect_1[j], Scalar(0, 0, 0));
						
						
						vector<Point2f> pp = reorder(rect_1[i]);
						vector<Point2f> jj = reorder(rect_1[j]);
						draw_rect(src,pp,jj,Scalar(0,0,255));
					}

				}
			}
		}


		


		//namedWindow("input", WINDOW_AUTOSIZE);// 显示窗口命名为input ；WINDOW_AUTOSIZE显示大小为图片自定义大小，且不可以更改大
		imshow("input", src); //显示
		imshow("1", gauss);
		imshow("2", binary);
		waitKey(0);
		//destroyAllWindows();
		return 0;

	}
}

bool funcu(RotatedRect n1, RotatedRect n2)
{
	return n1.center.y > n2.center.y;
}

bool func2(Rect n1, Rect n2)
{
	return (n1.br().y + n1.tl().y) > (n2.br().y + n2.tl().y);
}




//bool is_parell(RotatedRect rect1, RotatedRect rect2)
//{
//	Point2f P1[4], P2[4];
//	rect1.points(P1);
//	rect2.points(P2);
//	Point2f a;
//	a.x = P1[1].x - P1[2].x;
//	a.y = P1[1].y - P1[0].y;
//	Point2f b;
//	b.x = P2[1].x - P2[0].x;
//	b.y = P2[1].y - P2[0].y;
//
//	float cos = (a.x * b.x + a.y * b.y) / (sqrt(pow(a.x, 2) + pow(a.y, 2)) * sqrt(pow(b.x, 2) + pow(b.y, 2)));
//	if (
//		(abs(cos)<0.2 && abs(cos)>-0.2) || (abs(cos)<1.2 && abs(cos) > 0.8)
//
//		)
//	{
//		return true;
//	}
//	else 
//	{
//		return false;
//	}
//}

void draw_(Mat src, RotatedRect rect,Scalar ss) {
	Point2f P[400];
	rect.points(P);
	for (int j = 0; j <= 3; j++)
	{
		{
		//	line(src, P[j], P[(j + 1) % 4], ss, 1);
			line(src, P[j], P[(j + 1) % 4], ss, 2);
		}

	}


}




vector<Point2f> reorder(RotatedRect rect)
{
	vector<Point2f> points;
	Point2f P[4];
	rect.points(P);
	for (int j = 0; j <= 3; j++)
	{
		points.push_back(P[j]);

	}
	vector<Point2f> newPoints;
	vector<float> sumPoints, subPoints;
	for (int i = 0; i < 4; i++)
	{
		sumPoints.push_back(points[i].x + points[i].y);
		subPoints.push_back(points[i].x - points[i].y);

	}

	newPoints.push_back(points[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);
	newPoints.push_back(points[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
	newPoints.push_back(points[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
	newPoints.push_back(points[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);
	return newPoints;
}

void draw_rect(Mat src,vector<Point2f> pp, vector<Point2f> jj,Scalar ss)
{
	Point2f a = pp[0];
	Point2f b = pp[2];
	Point2f c = jj[3];
	Point2f d = jj[1];
	//	line(src, P[j], P[(j + 1) % 4], ss, 1);
	line(src, a, b, ss, 2);
	line(src, b, c, ss, 2);
	line(src, c, d, ss, 2);
	line(src, d, a, ss, 2);

}