#include "opencv2/opencv.hpp"
#include <iostream>

#include "fonctions.hpp"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
  Mat image,gray,thresh;
  vector< square_robotrovision> result;

  PARAM param;
  param.arclength_min=50;
  param.arclength_max=1000;
  param.arclength_tolerance=0.01;
  param.length_tolerance=0.05;
  param.angle_tolerance=0.05;
  param.min_angle=-10*M_PI/180;
  param.max_angle=10*M_PI/180;
  param.h_l=100;

  int gray_thresh=250,i;


  image = imread("shapes.png",-1);

  cvtColor(image,gray,COLOR_BGR2GRAY);
  imshow("gray",gray);

  threshold( gray, thresh, gray_thresh, 255,THRESH_BINARY_INV);
  imshow("thresh",thresh);

  find_squares(result,thresh,param);

  for(i=0;i<result[0].approx.size();i++) line(image, result[0].approx[i], result[0].approx[(i+1)%result[0].approx.size()], Scalar(0, 0, 255), 1, 8, 0);
  imshow("image",image);
  waitKey(0);
  return 0;
}
