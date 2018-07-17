#include<sstream>
#include "detector.h"

PieceDetector::PieceDetector(const std::string _params)
{
	// validate parameters file
	cv::FileStorage xml(_params,cv::FileStorage::READ);
	if( !xml.isOpened() )
	{
		// default parameters
		gui_=0;
		verbose_=1;
		wname_=std::string("piece detector");

		// warning message
		std::cerr << "[warning] \""+_params+"\" is not a valid parameters file, using default parameters" << std::endl;
	}
	else
	{
		// load parameters
		this->importConfig(xml);
	}
	
	// store file path
	params_=_params;
}

PieceDetector::~PieceDetector(void)
{
	// validate parameters file
	cv::FileStorage xml(params_,cv::FileStorage::WRITE);
	if( !xml.isOpened() )
	{
		// warning message
		std::cerr << "[warning] cannot update the parameters file \"" << params_ << "\"";
		return;
	}
	
	// store parameters
	this->exportConfig(xml);
}

void PieceDetector::imageOperator(Operator* _operator, const cv::Mat& _src, cv::Mat& _dst)
{
	if( gui_ )
	{
		cv::namedWindow(wname_+" gui",CV_WINDOW_NORMAL|CV_WINDOW_KEEPRATIO);
		_operator->gui(wname_+" gui");

		cv::Mat aux;
		for( char c=0; 0x1b!=c; c=cv::waitKey(30) )
		{
			_operator->apply(_src,aux);
			cv::imshow(wname_,aux);
		}
		aux.copyTo(_dst);

		cv::destroyWindow(wname_+" gui");
	}
	else
	{
		_operator->apply(_src,_dst);
		
		if( verbose_ )
		{
			for( char c=0; 0x1b!=c; c=cv::waitKey(30) )
			{
				cv::imshow(wname_,_dst);
			}
		}
	}
}

void PieceDetector::blurring(const cv::Mat& _src, cv::Mat& _dst)
{
	this->imageOperator((Operator*)&blur_,_src,_dst);
}

void PieceDetector::binarization(const cv::Mat& _src, cv::Mat& _dst)
{
	this->imageOperator((Operator*)&binary_,_src,_dst);
}

void PieceDetector::borderRemoval(const cv::Mat& _src, cv::Mat& _dst)
{
	this->imageOperator((Operator*)&border_,_src,_dst);
}

// morphological operations
void PieceDetector::closing(const cv::Mat& _src, cv::Mat& _dst)
{
	this->imageOperator((Operator*)&mclose_,_src,_dst);
}

void PieceDetector::opening(const cv::Mat& _src, cv::Mat& _dst)
{
	this->imageOperator((Operator*)&mopen_,_src,_dst);
}

// edges and contours detection
void PieceDetector::canny(const cv::Mat& _src, cv::Mat& _dst)
{
	this->imageOperator((Operator*)&canny_,_src,_dst);
}

void PieceDetector::suzuki(const cv::Mat& _src, cv::Contours& _contours, cv::Hierarchy& _hierarchy)
{
	cv::Mat aux;

	if( gui_ )
	{
		cv::namedWindow(wname_+" gui",CV_WINDOW_NORMAL|CV_WINDOW_KEEPRATIO);
		suzuki_.gui(wname_+" gui");

		for( char c=0; 0x1b!=c; c=cv::waitKey(30) )
		{
			_src.copyTo(aux);
			suzuki_.apply(aux,_contours,_hierarchy);

			aux=cv::Scalar(0);
			for( size_t i=0; _contours.size()>i; i++ )
			{
				cv::drawContours(aux,_contours,i,cv::Scalar(255),2,8,_hierarchy,0,cv::Point());
			}
			cv::imshow(wname_,aux);
		}

		cv::destroyWindow(wname_+" gui");
	}
	else
	{
		_src.copyTo(aux);
		suzuki_.apply(aux,_contours,_hierarchy);
		
		if( verbose_ )
		{
			aux=cv::Scalar(0);
			for( size_t i=0; _contours.size()>i; i++ )
			{
				cv::drawContours(aux,_contours,i,cv::Scalar(255),2,8,_hierarchy,0,cv::Point());
			}

			for( char c=0; 0x1b!=c; c=cv::waitKey(30) )
			{
				cv::imshow(wname_,aux);
			}
		}
	}
}

void PieceDetector::lines(const cv::Mat& _src, cv::Lines& _lines)
{
	cv::Mat aux(_src.rows,_src.cols,_src.depth());

	if( gui_ )
	{
		cv::namedWindow(wname_+" gui",CV_WINDOW_NORMAL|CV_WINDOW_KEEPRATIO);
		hough_.gui(wname_+" gui");
		lines_.gui(wname_+" gui");

		for( char c=0; 0x1b!=c; c=cv::waitKey(30) )
		{
			hough_.apply(_src,_lines);
			lines_.apply(_lines,_lines);

			aux=cv::Scalar(0);
			for( cv::Lines::iterator it=_lines.begin(); _lines.end()!=it; it++ )
			{
				cv::line(aux,cv::Point((*it)[0],(*it)[1]),cv::Point((*it)[2],(*it)[3]),cv::Scalar(255),1,CV_AA);
			}

			std::stringstream ss;
			ss << "number of lines: " << _lines.size();
			cv::putText(aux,ss.str(),cv::Point(20,60),cv::FONT_HERSHEY_SIMPLEX,1.0,cv::Scalar(255));

			cv::imshow(wname_,aux);
		}

		cv::destroyWindow(wname_+" gui");
	}
	else
	{
		hough_.apply(_src,_lines);
		lines_.apply(_lines,_lines);
		
		if( verbose_ )
		{
			aux=cv::Scalar(0);
			for( cv::Lines::iterator it=_lines.begin(); _lines.end()!=it; it++ )
			{
				cv::line(aux,cv::Point((*it)[0],(*it)[1]),cv::Point((*it)[2],(*it)[3]),cv::Scalar(255),1,CV_AA);
			}

			for( char c=0; 0x1b!=c; c=cv::waitKey(30) )
			{
				cv::imshow(wname_,aux);
			}
		}
	}
}

void PieceDetector::shapes(const cv::Mat& _src, const cv::Contours& _contours, cv::Shapes& _dst)
{
	cv::Mat aux(_src.rows,_src.cols,CV_8UC3);

	if( gui_ )
	{
		cv::namedWindow(wname_+" gui",CV_WINDOW_NORMAL|CV_WINDOW_KEEPRATIO);
		rectangle_.gui(wname_+" gui");

		for( char c=0; 0x1b!=c; c=cv::waitKey(30) )
		{
			// shapes initialization (empty)
			_dst.clear();

			// clear preview image
			aux=cv::Scalar(0,0,0);

			// for each contour,
			for( cv::Contours::const_iterator it=_contours.begin(); _contours.end()!=it; it++ )
			{
				// extract contour shape
				cv::Shape shape;
				rectangle_.apply(cv::Mat(*it),shape);

				// if the shape was found,
				if( 0<=shape.type )
				{
					// store contour shape
					_dst.push_back(shape);
				}

				// draw contour
				cv::drawContours(aux,_contours,(int)(it-_contours.begin()),cv::Scalar(255,255,255),CV_FILLED,CV_AA);

				// if the shape was found,
				if( 0<=shape.type )
				{
					// draw shape
					float dx=60.0f*cosf(shape.theta);
					float dy=60.0f*sinf(shape.theta);
					cv::line(aux,shape.center,shape.center+cv::Point(dx,dy),cv::Scalar(0,0,255),3,CV_AA);
					cv::line(aux,shape.center,shape.center-cv::Point(-dy,+dx),cv::Scalar(0,255,0),3,CV_AA);
					cv::circle(aux,shape.center,5.0f,cv::Scalar(255,0,0),1,CV_AA);
				}
			}

			cv::imshow(wname_,aux);
		}

		cv::destroyWindow(wname_+" gui");
	}
	else
	{
		// shapes initialization (empty)
		_dst.clear();

		if( verbose_ )
		{
			// clear preview image
			aux=cv::Scalar(0,0,0);
		}

		// for each contour,
		for( cv::Contours::const_iterator it=_contours.begin(); _contours.end()!=it; it++ )
		{
			// extract contour shape
			cv::Shape shape;
			rectangle_.apply(cv::Mat(*it),shape);

			// if the shape was found,
			if( 0<=shape.type )
			{
				// store contour shape
				_dst.push_back(shape);
			}


			if( verbose_ )
			{
				// draw contour
				cv::drawContours(aux,_contours,(int)(it-_contours.begin()),cv::Scalar(255,255,255),CV_FILLED,CV_AA);

				// if the shape was found,
				if( 0<=shape.type )
				{
					// draw shape
					float dx=60.0f*cosf(shape.theta);
					float dy=60.0f*sinf(shape.theta);
					cv::line(aux,shape.center,shape.center+cv::Point(dx,dy),cv::Scalar(0,0,255),3,CV_AA);
					cv::line(aux,shape.center,shape.center-cv::Point(-dy,+dx),cv::Scalar(0,255,0),3,CV_AA);
					cv::circle(aux,shape.center,5.0f,cv::Scalar(255,0,0),1,CV_AA);
				}
			}
		}
		
		for( char c=0; 0x1b!=c; c=cv::waitKey(30) )
		{
			cv::imshow(wname_,aux);
		}
	}
}

// export configuration parameters
void PieceDetector::exportConfig(cv::FileStorage& xml)
{
	xml << "gui" << gui_;
	xml << "verbosity" << verbose_;
	xml << "window_name" << wname_;
	
	blur_.exportConfig(xml);
	binary_.exportConfig(xml);
	border_.exportConfig(xml);
	mclose_.exportConfig(xml);
	mopen_.exportConfig(xml);
	canny_.exportConfig(xml);
	suzuki_.exportConfig(xml);
	hough_.exportConfig(xml);
	lines_.exportConfig(xml);
	rectangle_.exportConfig(xml);
}

// import configuration parameters
void PieceDetector::importConfig(cv::FileStorage& xml)
{
	gui_=(int)xml["gui"];
	verbose_=(int)xml["verbosity"];
	wname_=(std::string)xml["window_name"];
	
	blur_.importConfig(xml);
	binary_.importConfig(xml);
	border_.importConfig(xml);
	mclose_.importConfig(xml);
	mopen_.importConfig(xml);
	canny_.importConfig(xml);
	suzuki_.importConfig(xml);
	hough_.importConfig(xml);
	lines_.importConfig(xml);
	rectangle_.importConfig(xml);
}

const std::string& PieceDetector::windowName(void)
{
	return wname_;
}
