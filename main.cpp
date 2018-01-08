#include<sstream>
#include<eigen3/Eigen/Dense>
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
	typedef std::vector<cv::Vec4i> Lines;
};

int main(int aegc, char *argv[])
{
	std::string window_name("pieces detector");
    cv::namedWindow(window_name,cv::WINDOW_AUTOSIZE);

	cv::Mat img=cv::imread("../source2.jpg",cv::IMREAD_GRAYSCALE);

	// work in progress...

	cv::imshow(window_name,img);
	cv::waitKey(0);
	
	return 0;
}
