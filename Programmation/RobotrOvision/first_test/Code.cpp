#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

RNG rng(12345);

int main(int argc, char *argv[])
{
  Mat image,gray,thresh;
  vector< Vec4i > h;
  vector <Point> approx;
  vector< vector <Point> > contours;

  image = imread("shapes.png",-1);

  cvtColor(image,gray,COLOR_BGR2GRAY);
  imshow("gray",gray);

//ret,thresh = cv2.threshold(gray,127,255,1)
  threshold( gray, thresh, 200, 255,THRESH_BINARY_INV);
  imshow("thresh",thresh);

//contours,h = cv2.findContours(thresh,1,2)
  findContours 	( thresh,contours,h,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,Point(0,0));

  for( int i = 0; i< contours.size(); i++ )
  {
    approxPolyDP(contours[i], approx, 0.05*arcLength(contours[i],1), 1);
    //approx = cv2.approxPolyDP(cnt,0.01*cv2.arcLength(cnt,True),True)
    cout<< approx.size()<<endl;
    if (approx.size()==5)
        {cout<<"pentagon"<<endl;}
        //drawContours(image,approx,0,255,-1)
    else if (approx.size()==3)
        {cout<<"triangle"<<endl;}
        //drawContours(img,[cnt],0,(0,255,0),-1)
    else if (approx.size()==4)
        {cout<<"square"<<endl;}
        //drawContours(img,[cnt],0,(0,0,255),-1)
    else if (approx.size() == 9)
        {cout<<"half-circle"<<endl;}
      //  drawContours(img,[cnt],0,(255,255,0),-1)
    else
        {cout<< "circle" <<endl;}
    cout<<approx<<endl;
    //    drawContours(img,[cnt],0,(0,255,255),-1)
    //drawContours(image,approx,0,(0,255,0),-1);
      Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
      drawContours( image, contours, i, color, 2,8,h,0,Point() );
    }

  imshow("image",image);
  waitKey(0);
  return 0;
}
