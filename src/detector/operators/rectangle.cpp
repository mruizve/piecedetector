#include<eigen3/Eigen/Dense>
#include "operator.h"

// parameters data
static const cv::Parameters p[]=
{
	{3,0,"aspect_ratio"}, // 2/3 +/- 7.5e-(i+1), i in [0,3]
	{30,15,"threshold"}   // 0 if < threshold, 1 if > 255-threshold, undefined otherwise
};

Rectangle::Rectangle(void)
{
	// filter name
	name_=std::string("rectangle");

	// parameters definition
	p_=std::vector<cv::Parameters>(p,p+sizeof(p)/sizeof(cv::Parameters));
}

Rectangle::~Rectangle(void)
{
}

inline float d(cv::Point2f _p1, cv::Point2f _p2)
{
	return sqrtf(powf(_p1.x-_p2.x,2) + powf(_p1.y-_p2.y,2));
}

inline void correspondences(cv::Point2f _u[], float _d1, float _d2)
{
	_u[0]=cv::Point2f(0.0f,0.0f);
	_u[1]=cv::Point2f(_d1,0.0f);
	_u[2]=cv::Point2f(_d1,_d2);
	_u[3]=cv::Point2f(0.0f,_d2);
}

inline void shape(cv::Shape& _shape, int _type, const cv::Point2f _v[], int _idx)
{
	// shape type
	_shape.type=_type;

	// shape center
	cv::Point2f dx=_v[_idx-1]-_v[_idx];
	cv::Point2f dy=_v[(_idx+1)%4]-_v[_idx];
	_shape.center=_v[_idx]+0.50f*dx+0.25f*dy;

	// shape orientation
	_shape.theta=atan2(dx.y,dx.x);
}

void Rectangle::apply(const cv::Mat& _src, cv::Shape& _dst)
{
	// shape initialization (as "no type")
	_dst.type=-1;

	// compute minimum area rectangle surrounding the shape boundary
	cv::RotatedRect rect=minAreaRect(_src);

	// extract rectangle vertexes
	cv::Point2f v[4];
	rect.points(v);

	// based on the rectangle orientation,
	cv::Mat aligned;
	cv::Point2f u[4];
	if( d(v[0],v[1]) > d(v[1],v[2]) )
	{
		// generate point correspondences for warping
		correspondences(u,d(v[0],v[1]),d(v[1],v[2]));

		// initialize aligned canvas
		aligned=cv::Mat(d(v[1],v[2]),d(v[0],v[1]),CV_8UC3,cv::Scalar(0,0,0));
	}
	else
	{
		// reorder rectangle vertexes points
		cv::Point2f aux(v[0].x,v[0].y);
		for( int i=0; 3>i; i++)
		{
			v[i]=v[i+1];
		}
		v[3]=aux;

		// generate point correspondences for warping
		correspondences(u,d(v[1],v[2]),d(v[0],v[1]));

		// initialize aligned canvas
		aligned=cv::Mat(d(v[0],v[1]),d(v[1],v[2]),CV_8UC3,cv::Scalar(0,0,0));
	}

	// get normalization transformation
	cv::Mat H=getAffineTransform(v,u);

	// transform boundary points to normalized coordinates
	cv::Mat boundary;
	cv::transform(_src,boundary,H);

	// generate shape image from normalized boundary
	cv::Contours contours;
	contours.push_back(boundary);
	cv::drawContours(aligned,contours,0,cv::Scalar(255,255,255),CV_FILLED,CV_AA);

	// shape ratio validation
	if( fabs(aligned.cols/(double)aligned.rows-1.5) > 7.5*pow(10.0,-(p_[0].val+1.0)) )
	{
		return;
	}

	// descriptor computation
	int index=0;
	int descriptor=0;
	int width=aligned.cols/3;
	int height=aligned.rows/2;
	for( int j=0; 2>j; j++)
	{
		for( int i=0; 3>i; i++)
		{
			// region of interest (square block of the shape)
			cv::Rect roi(i*width,j*height,width,height);

			// mean intensity of the region
			int mean=cv::mean(aligned(roi)).val[0];

			// block's mean intensity validation
			if( p_[1].val<mean && (255-p_[1].val)>mean )
			{
				return;
			}

			// set the associated bit in the descriptor
			descriptor |= ((255-p_[1].val<mean)<<(5-index));
			index++;
		}
	}

	// shape identification
	switch( descriptor)
	{
		// inverted S
		case 0b00110011:
			shape(_dst,0,v,1);
			break;

		// right L (origin at top right corner)
		case 0b00100111:
			shape(_dst,1,v,3);
			break;

		// right L (origin at bottom left corner)
		case 0b00111001:
			shape(_dst,1,v,1);
			break;

		// T (origin at top right corner)
		case 0b00111010:
			shape(_dst,2,v,1);
			break;

		// T (origin at bottom left corner)
		case 0b00010111:
			shape(_dst,2,v,3);
			break;

		// S
		case 0b00011110:
			shape(_dst,3,v,1);
			break;

		// left L (origin at top right corner)
		case 0b00111100:
			shape(_dst,4,v,1);
			break;

		// left L (origin at bottom left corner)
		case 0b00001111:
			shape(_dst,4,v,3);
			break;
	}
}
