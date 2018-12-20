#include "operator.h"

// parameters data
static const cv::Parameters p[]=
{
	{10,2,"width"},
};

BorderMask::BorderMask(void)
{
	// filter name
	name_=std::string("border_mask");

	// parameters definition
	p_=std::vector<cv::Parameters>(p,p+sizeof(p)/sizeof(cv::Parameters));
}

BorderMask::~BorderMask(void)
{
}

void BorderMask::apply(const cv::Mat& _src, cv::Mat& _dst)
{
	cv::Mat mask=cv::Mat(_src.rows,_src.cols,_src.depth(),cv::Scalar(0));
	for( size_t i=0; mask.rows>i; i++ )
	{
		for( size_t j=0; (size_t)(p_[0].val)>=j; j++ )
		{
			mask.at<char>(i,j)=255;
			mask.at<char>(i,mask.cols-1-j)=255;
		}
	}
	for( size_t j=0; mask.cols>j; j++ )
	{
		for( size_t i=0; (size_t)(p_[0].val)>=i; i++ )
		{
			mask.at<char>(i,j)=255;
			mask.at<char>(mask.rows-1-i,j)=255;
		}
	}
	cv::bitwise_or(_src,mask,_dst);
}
