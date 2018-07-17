#include<stdexcept>
#include "detector.h"

static int cbOpenCVEx(int status, const char* func_name, const char* err_msg, const char* file_name, int line, void*)
{
    return 0;
}

int main(int argc, char *argv[])
{
	cvRedirectError(cbOpenCVEx);

	// validate input arguments
	if( 3!=argc )
	{
		std::cout << "usage: " << argv[0] << " parameters image" << std::endl;
		return -1;
	}

	// prepare detector object
	PieceDetector *detector;
	try
	{
		detector=new PieceDetector(argv[1]);
	}
	catch( const std::runtime_error& ex )
	{
		std::cerr << "[error] " << ex.what() << std::endl;
		return -1;
	}
	catch( const cv::Exception& ex )
	{
		std::cerr << "[error] " << ex.what();
		return -1;
	}

	// load image
	cv::Mat raw=cv::imread(argv[2],cv::IMREAD_GRAYSCALE);
	if( !raw.data )
	{
		std::cerr << "[error] \"" << argv[2] << "\" is not a valid input image" << std::endl;
		return -1;
	}

	// create visualization window
	std::string wname(detector->windowName());
	cv::namedWindow(wname,CV_WINDOW_AUTOSIZE|CV_WINDOW_KEEPRATIO);

	// show original image
	do
	{
		cv::imshow(wname,raw);
	} while( 0x1b!=(char)cv::waitKey(30) );

	// noise filtering
	cv::Mat img;
	detector->blurring(raw,img);

	// image binarization
	//detector->binarization(img,img);

	// morphological opening of the binary image
	//detector->opening(img,img);

	// border removal (to avoid failures during contours detection)
	//detector->borderRemoval(img,img);

	// edges detection
	cv::Mat edges;
	detector->canny(img,edges);

	// morphological closing of edges
	detector->closing(edges,edges);

	// contours detection
	cv::Contours contours;
	cv::Hierarchy hierarchy;
	detector->suzuki(edges,contours,hierarchy);

	cv::Shapes shapes;
	detector->shapes(edges,contours,shapes);

/*
	for( cv::Contours::iterator it=contours.begin(); contours.end()!=it; it++ )
	{
		// generate shape image
		cv::Mat piece(edges.rows,edges.cols,edges.depth(),cv::Scalar(0));
		cv::drawContours(piece,contours,(int)(it-contours.begin()),cv::Scalar(255),CV_FILLED,8,hierarchy,0,cv::Point());

		// detect lines
		cv::Lines lines;
		detector->lines(piece,lines);
	}
*/
	delete detector;
	
	return 0;
}
