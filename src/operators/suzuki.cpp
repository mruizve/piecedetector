#include "operator.h"

// parameters data
static const cv::Parameters p[]=
{
	{5000,250,"min_length"}
};

Suzuki::Suzuki(void)
{
	// filter name
	name_=std::string("suzuki");

	// parameters definition
	p_=std::vector<cv::Parameters>(p,p+sizeof(p)/sizeof(cv::Parameters));
}

Suzuki::~Suzuki(void)
{
}

void Suzuki::apply(const cv::Mat& _src, cv::Contours& _dst, cv::Hierarchy& _hierarchy)
{
	cv::findContours(_src,_dst,_hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);

	// remove "small" contours
	for( size_t i=0; _dst.size()>i; i++ )
	{
		if( _dst[i].size()<p_[0].val )
		{
			_dst.erase(_dst.begin()+i);
			_hierarchy.erase(_hierarchy.begin()+i);
			i--;
		}
	}
}
