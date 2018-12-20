#include "operator.h"

// parameters data
static const cv::Parameters p[]=
{
	{255,16,"threshold"},
	{255,255,"max_value"},
	{1,1,"type"}
};

Binarization::Binarization(void)
{
	// filter name
	name_=std::string("binarization");

	// parameters definition
	p_=std::vector<cv::Parameters>(p,p+sizeof(p)/sizeof(cv::Parameters));
}

Binarization::~Binarization(void)
{
}

void Binarization::apply(const cv::Mat& _src, cv::Mat& _dst)
{
	int type=(1==p_[2].val)?(cv::THRESH_BINARY):(cv::THRESH_BINARY_INV);
	cv::threshold(_src,_dst,p_[0].val,p_[1].val,type);
}
