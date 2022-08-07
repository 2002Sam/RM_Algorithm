#ifndef _POINT_UTIL_H_
#define _POINT_UTIL_H_
#include <opencv2/opencv.hpp>
#include"RMDefine.h"
class Util
{
private:
    static Util& instance()
    {
        static Util util;
        return util;
    }
    const cv::Mat StructuringElement3 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    const cv::Mat StructuringElement5 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    const cv::Mat StructuringElement7 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7));

public:
    static inline cv::Mat structuringElement3()
    {
        return instance().StructuringElement3;
    }
    static inline cv::Mat structuringElement5()
    {
        return instance().StructuringElement5;
    }
    static inline cv::Mat structuringElement7()
    {
        return instance().StructuringElement7;
    }
    static inline float findExtreamSide(cv::RotatedRect &rect,bool flag)
    {
        if (flag)
        {
            return rect.size.width < rect.size.height ? rect.size.height : rect.size.width;
        }
        else
        {
            return rect.size.width > rect.size.height ? rect.size.height : rect.size.width;
        }
    }

    static inline float findLineandLineAngle(cv::Point2f &p1, cv::Point2f &p2, cv::Point2f &p3, cv::Point2f &p4)
    {
        if (p1.x == p2.x) { p2.x += 1e-10f; }
        if (p3.x == p4.x) { p4.x += 1e-10f; }
        float tan1 = (p2.y - p1.y) / (p2.x - p1.x);
        float tan2 = (p4.y - p3.y) / (p4.x - p3.x);
        float angle1 = atanf(tan1) * 180.0f / float(CV_PI);
        float angle2 = atanf(tan2) * 180.0f / float(CV_PI);
        float ans = fabs(fabs(angle1 - angle2) - 90);
        return 90.0f-ans;
    }

    static inline float pointToPointDistance(cv::Point2f point1,cv::Point2f point2)
    {
        return sqrtf(powf(point1.x - point2.x, 2) + powf(point1.y - point2.y, 2));
    }

    static inline cv::RotatedRect boundArmor(const cv::RotatedRect &rect1 ,const cv::RotatedRect& rect2)
    {
        cv::Point2f center = (rect1.center + rect2.center) / 2;
        cv::RotatedRect wh_1 = rect1;
        cv::RotatedRect wh_2 = rect2;
        float longside1 = findExtreamSide(wh_1, LONG);
        float longside2 = findExtreamSide(wh_2, LONG);
        float x = fabs(rect1.center.x - rect2.center.x);
        float y = (longside1 + longside2) / 2;
        float angle = std::atan2(rect2.center.y - rect1.center.y, rect2.center.x - rect1.center.x);
        cv::RotatedRect rect(center, cv::Size(x, y), angle * 180.0f / CV_PI);

        return rect;
    }
};


#endif