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

  char names[5][256]={"orange","black","green","yellow","blue"},patern[10];
  string carac;

  vector< vector<int> > h(5, vector<int>(2));
  vector< vector<int> > s(5, vector<int>(2));
  vector< vector<int> > v(5, vector<int>(2));

  int i,n=h.size();

  ifstream file("config.txt", ios::in);
  if(!file)
  {
      cerr << "Can't open config.txt!" << endl;
      return 0;
  }
  file>>carac>>param.arclength_min>>carac>>param.arclength_max>>carac>>param.arclength_tolerance>>carac>>param.length_tolerance>>carac>>param.angle_tolerance>>carac>>param.min_angle>>carac>>param.max_angle>>carac>>param.h_l;
  param.min_angle=param.min_angle*M_PI/180;
  param.max_angle=param.max_angle*M_PI/180;
  file>>carac>>param_hsv.k_size_gauss>>carac>>param_hsv.sigma_gauss>>carac>>param_hsv.k_size_canny>>carac>>param_hsv.lowThreshold_canny>>carac>>param_hsv.ratio_canny;
  file>>carac>>carac>>carac>>carac>>carac>>carac;
  for(i=0;i<5;i++)
  {
    file>>carac>>h[i][0]>>h[i][1]>>s[i][0]>>s[i][1]>>v[i][0]>>v[i][1];
  }
  file.close();

  if(argc!=2)
  {
    cout<<"./Code image_file"<<endl;
    return 0;
  }

  image = imread(argv[1],-1);
  if(image.rows==0)
  {
    cout<<"Invalid input file"<<endl;
    return 0;
  }

  color_separation= separate_colors(image,h,s,v,param_hsv);
  for(i=0;i<n;i++)
  {
    global_result.push_back(find_squares(color_separation[i],param));
  }
  //print_global_result(global_result);

  //for(i=0;i<result[0].approx.size();i++) line(image, result[0].approx[i], result[0].approx[(i+1)%result[0].approx.size()], Scalar(0, 0, 255), 1, 8, 0);
  //imshow("image",image);
  //waitKey(0);
  cout<<find_patern(global_result,patern)<<endl;
  cout<<patern<<endl;
  return 0;
}
