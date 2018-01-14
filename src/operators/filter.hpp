#include "filters.h"

template<typename Tin, typename Tout>
Filter<Tin,Tout>::Filter(void)
{
}

template<typename Tin, typename Tout>
Filter<Tin,Tout>::~Filter(void)
{
}

template<typename Tin, typename Tout>
void Filter<Tin,Tout>::gui(const std::string _wname, const Tin& _src, Tout& _dst, va_list _ap)
{
	Tout aux;
	cv::namedWindow(_wname);
	
	for( size_t i=0; p_.size()>i; i++)
	{
		// FIXME: trackbars are wrongly initialized
		cv::createTrackbar(p_[i].name,_wname,NULL,p_[i].max,p_[i].cb,&p_[i].val);
	}

	char key=0x00;
	for( ; 0x1b!=key; )
	{
		// update and apply filter (virtual functions)
		this->update();
		this->apply(_src,aux,_ap);

		cv::imshow(_wname,aux);
		key=cv::waitKey(30);
	}

	cv::destroyWindow(_wname);
	
	_dst=Tout(aux);
}


template<typename Tin, typename Tout>
void Filter<Tin,Tout>::apply(const Tin& _src, Tout& _dst, va_list _ap)
{
}

template<typename Tin, typename Tout>
void Filter<Tin,Tout>::update(void)
{
}

template<typename Tin, typename Tout>
void Filter<Tin,Tout>::exportConfig(cv::FileStorage& _xml)
{
	// store parameters
	_xml << name_ << "{";
	for( size_t i=0; p_.size()>i; i++ )
	{
		_xml << p_[i].name << p_[i].val;
	}
	_xml << "}";
}

template<typename Tin, typename Tout>
void Filter<Tin,Tout>::importConfig(cv::FileStorage& _xml)
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

// instantiate filters templates
template void Filter<cv::Mat,cv::Mat>::gui(const std::string, const cv::Mat&, cv::Mat&, va_list);
template void Filter<cv::Mat,cv::Mat>::exportConfig(cv::FileStorage&);
template void Filter<cv::Mat,cv::Mat>::importConfig(cv::FileStorage&);

template void Filter<cv::Mat,cv::Contours>::gui(const std::string, const cv::Mat&, cv::Contours&, va_list);
template void Filter<cv::Mat,cv::Contours>::exportConfig(cv::FileStorage&);
template void Filter<cv::Mat,cv::Contours>::importConfig(cv::FileStorage&);

