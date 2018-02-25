#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;
using namespace cv;

#ifndef ROBOTROVISION_H
#define ROBOTROVISION_H

typedef struct{
  Point center;
  double side_length;
  vector <Point> approx;
}square_robotrovision;

typedef struct{
  double arclength_min;
  double arclength_max;
  double arclength_tolerance;
  double length_tolerance;
  double angle_tolerance;
  double min_angle;
  double max_angle;
  double h_l;
}PARAM;

void find_squares(vector< square_robotrovision> &result,Mat gray, PARAM param);//gray a thresholded image

#endif
