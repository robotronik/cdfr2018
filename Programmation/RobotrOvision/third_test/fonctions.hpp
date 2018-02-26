#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
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
  double arclength_min;//min and max of the contour
  double arclength_max;
  double arclength_tolerance;//precision of the approximated contour
  double length_tolerance;//for the 4 sides compared to avg
  double angle_tolerance;//for the 4 angles compared to 90
  double min_angle;//inclinaison angles
  double max_angle;
  double h_l;//length of the reference segement
}PARAM;

typedef struct{
  int k_size_gauss;
  int sigma_gauss;
  int k_size_canny;
  int lowThreshold_canny;
  int ratio_canny;
}PARAM_HSV;

vector< square_robotrovision> find_squares(Mat gray, PARAM param);//gray a thresholded image
vector< Mat > separate_colors(Mat rgb_image,vector< vector <int> > &h,vector< vector <int> > &s,vector< vector <int> > &v,PARAM_HSV param);
void print_global_result(vector< vector <square_robotrovision> > global_result);
int find_patern(vector< vector< square_robotrovision> > global_result, char *patern);

#endif
