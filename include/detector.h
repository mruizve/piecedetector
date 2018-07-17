#ifndef __PIECE_DETECTOR_H__
#define __PIECE_DETECTOR_H__

#include "operator.h"

class PieceDetector: public Operator
{
	public:
		PieceDetector(const std::string _params);
		~PieceDetector(void);

		// image filters
		void blurring(const cv::Mat& _src, cv::Mat& _dst);
		void binarization(const cv::Mat& _src, cv::Mat& _dst);
		void borderRemoval(const cv::Mat& _src, cv::Mat& _dst);

		// morphological operations
		void closing(const cv::Mat& _src, cv::Mat& _dst);
		void opening(const cv::Mat& _src, cv::Mat& _dst);

		// edges, contours and lines detection
		void canny(const cv::Mat& _src, cv::Mat& _dst);
		void suzuki(const cv::Mat& _src, cv::Contours& _dst, cv::Hierarchy& _hierarchy);

		// lines detection and clustering
		void lines(const cv::Mat& _src, cv::Lines& _dst);

		// shapes recognition
		void shapes(const cv::Mat& _src, const cv::Contours& _contours, cv::Shapes& _dst);

		// configuration parameters
		void exportConfig(cv::FileStorage& xml);
		void importConfig(cv::FileStorage& xml);
		
		// get visualization window name
		const std::string& windowName(void);

	protected:
		void imageOperator(Operator* _operator, const cv::Mat& _src, cv::Mat& _dst);

	protected:
		// parameters
		int gui_;
		int verbose_;
		std::string params_;
		std::string wname_;

		// image filters
		Blurring blur_;
		Binarization binary_;
		BorderMask border_;
		
		// morphological operations
		Closing mclose_;
		Opening mopen_;

		// edges and contours detection
		Canny canny_;
		Suzuki suzuki_;

		// lines detection and grouping
		Hough hough_;
		Lines lines_;

		// shapes
		Rectangle rectangle_;
};

#endif
