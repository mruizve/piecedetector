#include "operator.h"

// parameters data
static const cv::Parameters p[]=
{
	{10,0,"rho"},
	{100,0,"theta"},
	{250,29,"threshold"},
	{250,20,"min_length"},
	{100,10,"max_gap"}
};

Hough::Hough(void)
{
	// filter name
	name_=std::string("hough");

	// parameters definition
	p_=std::vector<cv::Parameters>(p,p+sizeof(p)/sizeof(cv::Parameters));
}

Hough::~Hough(void)
{
}

void Hough::apply(const cv::Mat& _src, cv::Lines& _dst)
{
	cv::HoughLinesP(_src,_dst,p_[0].val+1,(CV_PI+0.01*p_[1].val)/180.0,1+p_[2].val,p_[3].val,p_[4].val);
}
