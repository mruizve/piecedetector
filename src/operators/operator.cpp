#include "operator.h"

Operator::Operator(void)
{
}

Operator::~Operator(void)
{
}

void Operator::gui(const std::string _wname)
{
	for( size_t i=0; p_.size()>i; i++)
	{
		cv::createTrackbar(p_[i].name,_wname,&p_[i].val,p_[i].max,NULL,NULL);
	}
}

void Operator::apply(const cv::Mat& _src, cv::Mat& _dst)
{
}

void Operator::exportConfig(cv::FileStorage& _xml)
{
	// store parameters
	_xml << name_ << "{";
	for( size_t i=0; p_.size()>i; i++ )
	{
		_xml << p_[i].name << p_[i].val;
	}
	_xml << "}";
}

void Operator::importConfig(cv::FileStorage& _xml)
{
	// extract parameters map
	cv::FileNode node=_xml[name_];

	// load parameters
	for( size_t i=0; p_.size()>i; i++ )
	{
		p_[i].val=(double)node[p_[i].name];
		i++;
	}
}
