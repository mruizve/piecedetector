/*
 * =====================================================================================
 *
 *       Filename:  shapes.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/01/2018 20:30:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  shapes_INC
#define  shapes_INC
#include<vector>
#include<opencv2/core/core.hpp>
void init_std_shapes(std::vector< std::vector<cv::Point> >&standardShapes);
int whatShapeIs(std::vector< std::vector<cv::Point>  >&standardShapes,std::vector<cv::Point> contour,float* val);
cv::Point contourCentre(std::vector<cv::Point> contour);
#endif   /* ----- #ifndef shapes_INC  ----- */
