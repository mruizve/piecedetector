#include "operator.h"

// parameters data
static const cv::Parameters p[]=
{
	{2,0,"shape"},
	{15,3,"size"}
};

Closing::Closing(void)
{
	// filter name
	name_=std::string("morphology_closing");

	// parameters definition
	p_=std::vector<cv::Parameters>(p,p+sizeof(p)/sizeof(cv::Parameters));
}

Closing::~Closing(void)
{
}

void Closing::apply(const cv::Mat& _src, cv::Mat& _dst)
{
	int s=cv::MORPH_ELLIPSE;
	int k=p_[1].val;
	
	switch( p_[0].val )
	{
		case 0: s=cv::MORPH_ELLIPSE; break;
		case 1: s=cv::MORPH_RECT; break;
		case 2: s=cv::MORPH_CROSS; break;
	}

	cv::Mat e=cv::getStructuringElement(s,cv::Size(2*k+1,2*k+1),cv::Point(k,k));

    cv::dilate(_src,_dst,e);
    cv::erode(_dst,_dst,e);
}

Opening::Opening(void)
{
	// filter name
	name_=std::string("morphology_opening");

	// parameters definition
	p_=std::vector<cv::Parameters>(p,p+sizeof(p)/sizeof(cv::Parameters));
}

Opening::~Opening(void)
{
}

void Opening::apply(const cv::Mat& _src, cv::Mat& _dst)
{
	int s=cv::MORPH_ELLIPSE;
	int k=p_[1].val;
	
	switch( p_[0].val )
	{
		case 0: s=cv::MORPH_ELLIPSE; break;
		case 1: s=cv::MORPH_RECT; break;
		case 2: s=cv::MORPH_CROSS; break;
	}

	cv::Mat e=cv::getStructuringElement(s,cv::Size(2*k+1,2*k+1),cv::Point(k,k));

    cv::erode(_src,_dst,e);
    cv::dilate(_dst,_dst,e);
}
