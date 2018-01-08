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

int main(int argc, char *argv[])
{
	// validate input arguments
	if( 2!=argc )
	{
		std::cout << "usage: " << argv[0] << " input_image" << std::endl;
		return -1;
	}

	// load image
	cv::Mat img=cv::imread(argv[1],cv::IMREAD_GRAYSCALE);
	if( !img.data )
    {
        std::cout << "[error] \"" << argv[1] << "\" is not a valid input image" << std::endl;
        return -1;
    }

	// create visualization window
	std::string window_name(argv[0]);
    cv::namedWindow(window_name,cv::WINDOW_AUTOSIZE);

	// work in progress...

	cv::imshow(window_name,img);
	cv::waitKey(0);
	
	return 0;
}
