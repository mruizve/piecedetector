#ifndef __PIECE_DETECTOR_TYPES_H__
#define __PIECE_DETECTOR_TYPES_H__

#if 2<CV_MAJOR_VERSION
	#error add OpenCV 3 headers
#else
	#include<opencv2/opencv.hpp>
#endif

namespace cv
{
	typedef std::vector<cv::Point> Contour;
	typedef std::vector<Contour> Contours;
	typedef std::vector<cv::Vec4i> Hierarchy;
	typedef cv::Vec4i Line;
	typedef std::vector<Line> Lines;

	struct Parameters
	{
		int max;          // maximum value
		int val;          // value
		std::string name; // name
	};
};

#endif
