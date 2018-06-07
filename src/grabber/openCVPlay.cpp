#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

int main(int argc, char* argv[])
{
	if( 2!=argc )
	{
		std::cout << "usage: " << argv[0] << "path_to_video" << std::endl;
	}

	cv::Mat frame;
	cv::VideoCapture video(argv[1]);
	cv::namedWindow("OpenCV Display Window", CV_WINDOW_AUTOSIZE);

	video >> frame;
	while( 0x1b!=(char)cv::waitKey(50) && !frame.empty() )
	{
		imshow("OpenCV Display Window", frame);
		video >> frame;
    }

	video.release();
	cv::destroyAllWindows();

    return 0;
}
