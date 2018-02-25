#include "fonctions.hpp"


int check_length(vector <Point> approx,vector <double> &l,double length_tolerance)//0 if not ok, avg at the end of length vector
{
  double avg=0,avg_max,avg_min;
  int n=approx.size(),i;
  for(i=0;i<n;i++)
  {
    l.push_back(sqrt(pow(approx[i].x-approx[(i+1)%n].x,2)+pow(approx[i].y-approx[(i+1)%n].y,2)));
    avg+=l[i];
  }
  avg=avg/4;
  l.push_back(avg);

  avg_max=avg*(1+length_tolerance);
  avg_min=avg*(1-length_tolerance);
  for(i=0;i<n;i++)
  {
    if(l[i]<avg_min || l[i]>avg_max) return 0;
  }
  return 1;
}

int check_angle(vector <Point> approx,vector <double> l,vector <double> &a,double angle_tolerance,double min_angle,double max_angle,double h_l)//0 if not ok, avg at the end of length vector
{
  double avg=0,avg_max,avg_min,horizontal_angle;
  int n=approx.size(),i;
  for(i=0;i<n;i++)
  {
    a.push_back(acos(((approx[(i+1)%n].x-approx[i].x)*(approx[(i+2)%n].x-approx[(i+1)%n].x)+(approx[(i+1)%n].y-approx[i].y)*(approx[(i+2)%n].y-approx[(i+1)%n].y))/(l[i]*l[(i+1)%n])));
    avg+=a[i];
  }
  avg=avg/4;
  a.push_back(avg);

  avg_max=avg*(1+angle_tolerance);
  avg_min=avg*(1-angle_tolerance);
  for(i=0;i<n;i++)
  {
    if(a[i]<avg_min || a[i]>avg_max) return 0;
  }
  i=0;
  horizontal_angle=acos((approx[(i+1)%n].x-approx[i].x)*h_l/(l[i]*h_l));
  if(horizontal_angle<max_angle && horizontal_angle>min_angle) return 1;//acos values are between 0 and M_PI
  horizontal_angle=horizontal_angle-M_PI/2;
  if(horizontal_angle<max_angle && horizontal_angle>min_angle) return 1;
  horizontal_angle=horizontal_angle-M_PI/2;
  if(horizontal_angle<max_angle && horizontal_angle>min_angle) return 1;
  return 0;
}

void find_squares(vector< square_robotrovision> &result,Mat gray, PARAM param)
{
  vector< Vec4i > h;
  vector <Point> approx;
  vector< vector <Point> > contours,approx_cont;
  vector < double > l,a;
  square_robotrovision r;

  findContours 	( gray,contours,h,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,Point(0,0));
  cout<<"contours found "<<contours.size()<<endl<<endl;
  for( int i = 0; i< contours.size(); i++ )
  {
      if(arcLength(contours[i],1)>param.arclength_min && arcLength(contours[i],1)<param.arclength_max)
      {
        cout<<"arcLength ok "<<arcLength(contours[i],1)<<endl;
        approxPolyDP(contours[i], approx, param.arclength_tolerance*arcLength(contours[i],1), 1);
        if (approx.size()==4)
        {
          cout<<"4 segments"<<endl;
          cout<<approx<<endl;
          if(check_length(approx,l,param.length_tolerance))
          {
            cout<<"length_tolerance ok avg "<<l[4]<<endl;
            if(check_angle(approx,l,a,param.angle_tolerance,param.min_angle,param.max_angle,param.h_l))
            {
              cout<<"angle_tolerance ok avg "<<a[4]<<endl<<"It is a squarre"<<endl;
              r.center.x=(approx[0].x+approx[1].x+approx[2].x+approx[3].x)/4;
              r.center.y=(approx[0].x+approx[1].x+approx[2].x+approx[3].x)/4;
              r.side_length=l[4];
              r.approx=approx;
              result.push_back(r);
            }
          }
        }
        cout<<endl;
      }
    }
}
