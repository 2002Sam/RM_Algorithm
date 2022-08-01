#include<iostream>
#include<opencv2/opencv.hpp>
#include<string>
#include<algorithm>
#include<math.h>
#include<cmath>

using namespace cv;
using namespace std;

const String path = "C:/Users/86133/Desktop/zhuanjiaban/";
const int kThreshold = 240;
const int kmax = 255;
const Size gsize = Size(5, 5);


vector<Vec4i> hierarchy;

vector<vector<Point>> contours;


Mat binary, gauss,dil,kernel;


void draw_(Mat src, RotatedRect rect,Scalar ss);

bool funcu(RotatedRect n1, RotatedRect n2);

bool func2(Rect n1, Rect n2);

bool is_parell(RotatedRect rect1, RotatedRect rect2);

vector<Point2f> reorder(RotatedRect rect);

void draw_rect(Mat src, vector<Point2f> pp, vector<Point2f>jj, Scalar ss);