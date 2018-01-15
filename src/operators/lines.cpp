#include<eigen3/Eigen/Dense>
#include "operator.h"

// parameters data
static const cv::Parameters p[]=
{
	{1,1,"merging"},  // enable/disable flag
	{100,90,"angle"}, // minimum alignment between lines
	{100,5,"error"},  // maximum error (in pixels)
	{250,35,"length"} // minimum line length (in pixels)
};

Lines::Lines(void)
{
	// filter name
	name_=std::string("lines_merging");

	// parameters definition
	p_=std::vector<cv::Parameters>(p,p+sizeof(p)/sizeof(cv::Parameters));
}

Lines::~Lines(void)
{
}

static inline double pNorm(const Eigen::MatrixXd _P, const Eigen::Vector3d _l)
{
	Eigen::Vector3d l=_l; l/=l(2);
	return (_P*l/l.head(2).norm()).norm();
}

void Lines::apply(const cv::Lines& _src, cv::Lines& _dst)
{
	if( !p_[0].val )
	{
		return;
	}

	double min_angle=p_[1].val/100.0;
	double max_error=p_[2].val;
	double min_length=p_[3].val;

	cv::Lines aux=cv::Lines(_src);
	_dst.clear();
	
	for( cv::Lines::iterator jt=aux.begin(); aux.end()!=jt; )
	{
		std::vector<Eigen::Vector3d> points;

		// points in homogeneous coordinates ("first" line)
		Eigen::Vector3d p; p << (*jt)[0], (*jt)[1], 1.0;
		Eigen::Vector3d q; q << (*jt)[2], (*jt)[3], 1.0;

		// "first" line in homogeneous coordinates
		Eigen::Vector3d l1=p.cross(q); l1/=l1(2);

		// add points of the "first" line to the cluster
		points.push_back(p);
		points.push_back(q);
		
		for( cv::Lines::iterator kt=jt+1; aux.end()!=kt; kt++ )
		{
			// points in homogeneous coordinates ("second" line)
			Eigen::Vector3d r; r << (*kt)[0], (*kt)[1], 1.0;
			Eigen::Vector3d s; s << (*kt)[2], (*kt)[3], 1.0;

			// "second" line in homogeneous coordinates
			Eigen::Vector3d l2=r.cross(s); l2/=l2(2);

			// compute angle and error between lines
			double n1=l1.head(2).norm();
			double n2=l2.head(2).norm();
			double a=fabs(l1.head(2).dot(l2.head(2)))/(n1*n2);
			Eigen::Vector4d e;
			e <<
				0.25*fabs(r.dot(l1))/n1, 0.25*fabs(s.dot(l1))/n1,
				0.25*fabs(p.dot(l2))/n2, 0.25*fabs(q.dot(l2))/n2;

			// if lines are relatively close and almost parallel, then
			if( min_angle<a && max_error>e.lpNorm<Eigen::Infinity>() )
			{
				// add points of the "second" line to the array
				points.push_back(r);
				points.push_back(s);
				
				// remove the "second" line from the list of lines
				kt=aux.erase(kt)-1;
			}
		}

		// remove the "first" line from the list of lines
		jt=aux.erase(jt);

		// matrix of points (homogeneous line fitting data)
		Eigen::MatrixXd P(points.size(),3);
		for( size_t i=0; points.size()>i; i++ )
		{
			P.row(i)=points[i].transpose();
		}

		// homogeneous line fitting
		Eigen::Matrix3d A=P.transpose()*P;
		Eigen::JacobiSVD<Eigen::Matrix3d> svd(A,Eigen::ComputeFullU|Eigen::ComputeFullV);
		Eigen::Vector3d l=svd.matrixV().col(2); l/=l(2);

		// it is not clear to me why sometimes the above solution
		// fails, but under uncertain circumstances the singular-vector
		// corresponding to the minimum singular-value of A is not the
		// correct solution of the best fitting line
		if( pNorm(P,svd.matrixV().col(2))>pNorm(P,svd.matrixV().col(1)) )
		{
			l=svd.matrixV().col(1); l/=l(1);
		}

		// bounding box (lines in homogeneous coordinates)
		Eigen::Vector3d ll; ll << -1.0/P.col(0).minCoeff(), 0.0, 1.0;
		Eigen::Vector3d lr; lr << -1.0/P.col(0).maxCoeff(), 0.0, 1.0;
		Eigen::Vector3d lb; lb << 0.0, -1.0/P.col(1).minCoeff(), 1.0;
		Eigen::Vector3d lt; lt << 0.0, -1.0/P.col(1).maxCoeff(), 1.0;

		// line clipping to the bounding box
		Eigen::Vector3d pl=l.cross(ll); pl/=pl(2);
		Eigen::Vector3d pr=l.cross(lr); pr/=pr(2);
		Eigen::Vector3d pb=l.cross(lb); pb/=pb(2);
		Eigen::Vector3d pt=l.cross(lt); pt/=pt(2);
		
		Eigen::Vector3d p1=pl;
		Eigen::Vector3d p2=pr;
		if( 0.0<(pb-pl).dot(pt-pl) )
		{
			p1=pt;
			p2=pb;
		}

		// if the line long enough, then
		if( min_length<(p2-p1).norm() )
		{
			// add line to the array
			_dst.push_back(cv::Line(p1(0),p1(1),p2(0),p2(1)));
		}
	}
}
