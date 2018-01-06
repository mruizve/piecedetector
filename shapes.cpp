/*
 * =====================================================================================
 *
 *       Filename:  shapes.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/01/2018 20:31:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include"shapes.h"

void init_std_shapes(std::vector< std::vector<cv::Point> >&standardShapes){
  const float ff =6.2f;
  std::vector<cv::Point> shape;
  cv::Point x0,x1;
  const float step = 0.05f;
  float t;
#define ADD_FIRST_INT_POINTS() for(t=0;t<1.0f;t+=step)  shape.push_back(x0+t*(x1-x0));
#define ADD_INT_POINTS(x,y) x0=x1; x1=cv::Point(x,y);  for(t=0;t<1.0f;t+=step)  shape.push_back(x0+t*(x1-x0));
//   _
//  | |___
//  |_____|
  shape.clear();
  x0 = cv::Point(0.0f,0.0f);
  x1 = cv::Point(60.0f,0.0f)*ff;
  ADD_FIRST_INT_POINTS()
  ADD_INT_POINTS(60.0f*ff,20.0f*ff);
  ADD_INT_POINTS(20.0f*ff,20.0f*ff);
  ADD_INT_POINTS(20.0f*ff,40.0f*ff);
  ADD_INT_POINTS( 0.0f*ff,40.0f*ff);
  ADD_INT_POINTS( 0.0f*ff, 0.0f*ff);

  standardShapes.push_back(shape);

//   _   _
//   ___| |
//  |_____|
  shape.clear();
  x0 = cv::Point(0.0f,0.0f);
  x1 = cv::Point(60.0f,0.0f)*ff;
  ADD_FIRST_INT_POINTS()
  ADD_INT_POINTS(60.0f*ff,40.0f*ff);
  ADD_INT_POINTS(40.0f*ff,40.0f*ff);
  ADD_INT_POINTS(40.0f*ff,20.0f*ff);
  ADD_INT_POINTS( 0.0f*ff,20.0f*ff);
  ADD_INT_POINTS( 0.0f*ff, 0.0f*ff);

  standardShapes.push_back(shape);

////     __
////   _| __|
////  |___|
  shape.clear();
  x0 = cv::Point(0.0f,0.0f);
  x1 = cv::Point(40.0f,0.0f)*ff;
  ADD_FIRST_INT_POINTS()
  ADD_INT_POINTS(40.0f*ff,20.0f*ff);
  ADD_INT_POINTS(60.0f*ff,20.0f*ff);
  ADD_INT_POINTS(60.0f*ff,40.0f*ff);
  ADD_INT_POINTS(20.0f*ff,40.0f*ff);
  ADD_INT_POINTS(20.0f*ff,20.0f*ff);
  ADD_INT_POINTS( 0.0f*ff,20.0f*ff);
  ADD_INT_POINTS( 0.0f*ff, 0.0f*ff);

  standardShapes.push_back(shape);

//// ___    
////|_  |_
////  |___|
  shape.clear();
  x0 = cv::Point(0.0f, 40.0f)*ff;
  x1 = cv::Point(0.0f, 20.0f)*ff;
  ADD_FIRST_INT_POINTS()
  ADD_INT_POINTS(20.0f*ff,20.0f*ff);
  ADD_INT_POINTS(20.0f*ff, 0.0f*ff);
  ADD_INT_POINTS(60.0f*ff, 0.0f*ff);
  ADD_INT_POINTS(60.0f*ff,20.0f*ff);
  ADD_INT_POINTS(40.0f*ff,20.0f*ff);
  ADD_INT_POINTS(40.0f*ff,40.0f*ff);
  ADD_INT_POINTS( 0.0f*ff,40.0f*ff);

  standardShapes.push_back(shape);


//       _
////   _| |_
////  |_____|
  shape.clear();
  x0 = cv::Point(0.0f,0.0f);
  x1 = cv::Point(60.0f,0.0f)*ff;
  ADD_FIRST_INT_POINTS()
//  shape.push_back(cv::Point(60.0f*ff,20.0f*ff));
  ADD_INT_POINTS(60.0f*ff,20.0f*ff);
  ADD_INT_POINTS(40.0f*ff,20.0f*ff);
  ADD_INT_POINTS(40.0f*ff,40.0f*ff);
  ADD_INT_POINTS(20.0f*ff,40.0f*ff);
  ADD_INT_POINTS(20.0f*ff,20.0f*ff);
  ADD_INT_POINTS( 0.0f*ff,20.0f*ff);
  ADD_INT_POINTS( 0.0f*ff,0.0f*ff);

  standardShapes.push_back(shape);

#undef ADD_FIRST_INT_POINTS
#undef ADD_INT_POINTS
}

int whatShapeIs(std::vector< std::vector<cv::Point>  >&standardShapes,std::vector<cv::Point> contour,float* val){
  
  size_t i,res_i;
  float res=1.1;
  float match1,match2,match3;
  float match_tot;
  for(i=0;i<standardShapes.size();i++){
    match1 = cv::matchShapes  (contour,standardShapes[i],CV_CONTOURS_MATCH_I1,0.0);
    match2 = cv::matchShapes  (contour,standardShapes[i],CV_CONTOURS_MATCH_I2,0.0);
    match3 = cv::matchShapes  (contour,standardShapes[i],CV_CONTOURS_MATCH_I3,0.0);
    match_tot = match1+match2+match3;
    if(match_tot<res){
      res=match_tot;
      res_i = i;
    }

  }

  if(res>1.0){
    *val=-1.0f;
    return -1;
  }

  if(val!=NULL)
    *val=res;

  return res_i;
}
cv::Point contourCentre(std::vector<cv::Point> contour){
  cv::Moments m;
  m = cv::moments(contour);
  return cv::Point(m.m10/m.m00,m.m01/m.m00);
}
