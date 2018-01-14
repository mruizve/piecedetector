#include "operator.h"

// parameters data
static const cv::Parameters p[]=
{
	{1500,50,"threshold1"},
	{1500,100,"threshold2"},
	{2,0,"aperture"},
	{1,0,"L2-norm"}
};

Canny::Canny(void)
{
	// filter name
	name_=std::string("canny");

	// parameters definition
	p_=std::vector<cv::Parameters>(p,p+sizeof(p)/sizeof(cv::Parameters));
}

Canny::~Canny(void)
{
}

void Canny::apply(const cv::Mat& _src, cv::Mat& _dst)
{
	cv::Canny(_src,_dst,p_[0].val,p_[1].val,2*p_[2].val+3,p_[3].val);
}
