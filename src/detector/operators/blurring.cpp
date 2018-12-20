#include "operator.h"

// parameters data
static const cv::Parameters p[]=
{
	{15,2,"ksize"},
	{5000,5000,"sigma"},
};

Blurring::Blurring(void)
{
	// filter name
	name_=std::string("blurring");

	// parameters definition
	p_=std::vector<cv::Parameters>(p,p+sizeof(p)/sizeof(cv::Parameters));
}

Blurring::~Blurring(void)
{
}

void Blurring::apply(const cv::Mat& _src, cv::Mat& _dst)
{
	cv::GaussianBlur(_src,_dst,cv::Size(2*p_[0].val+1,2*p_[0].val+1),p_[1].val/100.0);
}
