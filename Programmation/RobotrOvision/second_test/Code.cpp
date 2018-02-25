#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

RNG rng(12345);

typedef struct{
  Point center;
  double side_length;
}squarre_robotrovision;

int main(int argc, char *argv[])
{
  Mat image,gray,thresh;
  vector< Vec4i > h;
  vector <Point> approx;
  vector< vector <Point> > contours,approx_cont;
  squarre_robotrovision result;

  vector< squarre_robotrovision> result_vector;
  double l1,l2,l3,l4,lavg,lavg_min,lavg_max;//length and angles for squarre check
  double angle1,angle2,angle3,angle4,angleavg,angleavg_min,angleavg_max;

  double arclength_min=50,arclength_max=1000,arclength_tolerance=0.01,length_tolerance=0.05,angle_tolerance=0.05;//TODO parameters
  int gray_thresh=250;

  Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

  image = imread("test.jpg",-1);

  cvtColor(image,gray,COLOR_BGR2GRAY);
  imshow("gray",gray);

  threshold( gray, thresh, gray_thresh, 255,THRESH_BINARY_INV);
  imshow("thresh",thresh);

  findContours 	( thresh,contours,h,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,Point(0,0));
  cout<<"contours found "<<contours.size()<<endl<<endl;
  for( int i = 0; i< contours.size(); i++ )
  {
      cout<<arcLength(contours[i],1)<<endl;
      if(arcLength(contours[i],1)>arclength_min && arcLength(contours[i],1)<arclength_max)
      {
        approxPolyDP(contours[i], approx, arclength_tolerance*arcLength(contours[i],1), 1);
        if (approx.size()==4)
        {
          cout<<approx<<endl;
          cout<<"4 segments"<<endl;
          l1=sqrt(pow(approx[0].x-approx[1].x,2)+pow(approx[0].y-approx[1].y,2));
          l2=sqrt(pow(approx[1].x-approx[2].x,2)+pow(approx[1].y-approx[2].y,2));
          l3=sqrt(pow(approx[2].x-approx[3].x,2)+pow(approx[2].y-approx[3].y,2));
          l4=sqrt(pow(approx[3].x-approx[0].x,2)+pow(approx[3].y-approx[0].y,2));
          lavg=(l1+l2+l3+l4)/4;
          lavg_max=lavg*(1+length_tolerance);
          lavg_min=lavg*(1-length_tolerance);
          cout<<l1<<" "<<l2<<" "<<l3<<" "<<l4<<endl;
          if(l1>lavg_min && l1<lavg_max && l2>lavg_min && l2<lavg_max && l3>lavg_min && l3<lavg_max && l4>lavg_min && l4<lavg_max)
          {
            cout<<"length_tolerance ok"<<endl;
            angle1=acos((l1*l1+l2*l2-pow(approx[0].x-approx[2].x,2)-pow(approx[0].y-approx[2].y,2))/(2*l1*l2));
            angle2=acos((l2*l2+l3*l3-pow(approx[1].x-approx[3].x,2)-pow(approx[1].y-approx[3].y,2))/(2*l2*l3));
            angle3=acos((l3*l3+l4*l4-pow(approx[2].x-approx[0].x,2)-pow(approx[2].y-approx[0].y,2))/(2*l3*l4));
            angle4=acos((l4*l4+l1*l1-pow(approx[3].x-approx[1].x,2)-pow(approx[3].y-approx[1].y,2))/(2*l4*l1));
            angleavg=(angle1+angle2+angle3+angle4)/4;
            angleavg_max=angleavg*(1+angle_tolerance);
            angleavg_min=angleavg*(1-angle_tolerance);
            cout<<angle1<<" "<<angle2<<" "<<angle3<<" "<<angle4<<endl;
            if(angle1>angleavg_min && angle1<angleavg_max && angle2>angleavg_min && angle2<angleavg_max && angle3>angleavg_min && angle3<angleavg_max && angle4>angleavg_min && angle4<angleavg_max)
            {
              cout<<"angle_tolerance ok"<<endl<<"It is a squarre"<<endl;
              result.center.x=(approx[0].x+approx[1].x+approx[2].x+approx[3].x)/4;
              result.center.y=(approx[0].x+approx[1].x+approx[2].x+approx[3].x)/4;
              result.side_length=lavg;
              result_vector.push_back(result);

              approx_cont.push_back(approx);
              drawContours( image, approx_cont, 0, color, 2,8,h,0,Point() );
              approx_cont.pop_back();
              cout<<endl;
            }
          }
        }
      }
    }

  imshow("image",image);
  waitKey(0);
  return 0;
}
