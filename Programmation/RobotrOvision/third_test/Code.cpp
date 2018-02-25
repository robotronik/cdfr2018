#include "opencv2/opencv.hpp"
#include <iostream>

#include "fonctions.hpp"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
  Mat image,gray,thresh;
  vector< square_robotrovision> result;
  vector< vector <square_robotrovision> > global_result;
  vector< Mat > color_separation;

  PARAM param;
  PARAM_HSV param_hsv;

  param.arclength_min=50;
  param.arclength_max=1000;
  param.arclength_tolerance=0.01;
  param.length_tolerance=0.05;
  param.angle_tolerance=0.05;
  param.min_angle=-10*M_PI/180;
  param.max_angle=10*M_PI/180;
  param.h_l=100;

  param_hsv.s_min=50;
  param_hsv.s_max=255;
  param_hsv.v_min=50;
  param_hsv.v_max=255;
  param_hsv.k_size_gauss=9;
  param_hsv.sigma_gauss=2;
  param_hsv.k_size_canny=3;
  param_hsv.lowThreshold_canny=50;
  param_hsv.ratio_canny=3;

  vector< vector<int> > h(2, vector<int>(2));
  h[0][0]=160;
  h[0][1]=20;
  h[1][0]=100;
  h[1][1]=140;

  int i,n=h.size();


  image = imread("test.jpg",-1);

  color_separation= separate_colors(image,h,param_hsv);
  for(i=0;i<n;i++)
  {
    global_result.push_back(find_squares(color_separation[i],param));
  }
  print_global_result(global_result);
  //for(i=0;i<result[0].approx.size();i++) line(image, result[0].approx[i], result[0].approx[(i+1)%result[0].approx.size()], Scalar(0, 0, 255), 1, 8, 0);
  //imshow("image",image);
  waitKey(0);
  return 0;
}
