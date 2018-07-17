#ifndef __PIECE_DETECTOR_FILTERS_H__
#define __PIECE_DETECTOR_FILTERS_H__

#include "types.h"

class Operator
{
	public:
		Operator(void);
		virtual ~Operator(void);
		
		void gui(const std::string _wname);
		virtual void apply(const cv::Mat& _src, cv::Mat& _dst);

		void exportConfig(cv::FileStorage& xml);
		void importConfig(cv::FileStorage& xml);

	protected:
		std::string name_;
		std::vector<cv::Parameters> p_;
};

// image filters
class Blurring:  public Operator
{
	public:
		Blurring(void);
		~Blurring(void);
		void apply(const cv::Mat& _src, cv::Mat& _dst);
};

class Binarization: public Operator
{
	public:
		Binarization(void);
		~Binarization(void);
		void apply(const cv::Mat&, cv::Mat&);
};

class BorderMask: public Operator
{
	public:
		BorderMask(void);
		~BorderMask(void);
		void apply(const cv::Mat&, cv::Mat&);
};

// morphological operations
class Closing: public Operator
{
	public:
		Closing(void);
		~Closing(void);
		void apply(const cv::Mat&, cv::Mat&);
};

class Opening: public Operator
{
	public:
		Opening(void);
		~Opening(void);
		void apply(const cv::Mat&, cv::Mat&);
};

// edges, contours and lines detection
class Canny: public Operator
{
	public:
		Canny(void);
		~Canny(void);
		void apply(const cv::Mat&, cv::Mat&);
};

class Suzuki: public Operator
{
	public:
		Suzuki(void);
		~Suzuki(void);
		void apply(const cv::Mat&, cv::Contours&, cv::Hierarchy&);
};

class Hough: public Operator
{
	public:
		Hough(void);
		~Hough(void);
		void apply(const cv::Mat&, cv::Lines&);
};

class Lines: public Operator
{
	public:
		Lines(void);
		~Lines(void);
		void apply(const cv::Lines&, cv::Lines&);
};

// shape analysis
class Rectangle: public Operator
{
	public:
		Rectangle(void);
		~Rectangle(void);
		void apply(const cv::Mat&, cv::Shape&);
};

#endif
