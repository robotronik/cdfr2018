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

vector< square_robotrovision> find_squares(Mat gray, PARAM param)
{
  vector< Vec4i > h;
  vector <Point> approx;
  vector< vector <Point> > contours,approx_cont;
  vector < double > l,a;
  square_robotrovision r;
  vector< square_robotrovision> result;

  findContours 	( gray,contours,h,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,Point(0,0));
  //cout<<"contours found "<<contours.size()<<endl<<endl;
  for( int i = 0; i< contours.size(); i++ )
  {
      if(arcLength(contours[i],1)>param.arclength_min && arcLength(contours[i],1)<param.arclength_max)
      {
        //cout<<"arcLength ok "<<arcLength(contours[i],1)<<endl;
        approxPolyDP(contours[i], approx, param.arclength_tolerance*arcLength(contours[i],1), 1);
        if (approx.size()==4)
        {
          //cout<<"4 segments"<<endl;
          //cout<<approx<<endl;
          if(check_length(approx,l,param.length_tolerance))
          {
            //cout<<"length_tolerance ok avg "<<l[4]<<endl;
            if(check_angle(approx,l,a,param.angle_tolerance,param.min_angle,param.max_angle,param.h_l))
            {
              //cout<<"angle_tolerance ok avg "<<a[4]<<endl<<"It is a squarre"<<endl;
              r.center.x=(approx[0].x+approx[1].x+approx[2].x+approx[3].x)/4;
              r.center.y=(approx[0].y+approx[1].y+approx[2].y+approx[3].y)/4;
              r.side_length=l[4];
              r.approx=approx;
              result.push_back(r);
            }
          }
        }
      //  cout<<endl;
      }
    }
    return result;
}


vector<Mat> separate_colors(Mat rgb_image,vector<vector <int> > &h,vector< vector <int> > &s,vector< vector <int> > &v,PARAM_HSV param)
{
  vector<Mat> result;
  Mat frame_gray;
  Mat1b mask1, mask2;
  Mat3b hsvframe;
  Mat1b mask;
  int n,i,min_H,max_H;
  char chaine[256];
  char names[5][256]={"orange","black","green","yellow","blue"};

  n=h.size();
  cvtColor(rgb_image, hsvframe, COLOR_BGR2HSV);

  for(i=0;i<n;i++)
  {
    min_H=h[i][0];
    max_H=h[i][1];
    if(min_H<=max_H)
    {
      inRange(hsvframe, Scalar(min_H, s[i][0], v[i][0]), Scalar(max_H, s[i][1], v[i][1]), mask);
    }
    else
    {
      inRange(hsvframe, Scalar(min_H, s[i][0], v[i][0]), Scalar(180,  s[i][1], v[i][1]), mask1);
      inRange(hsvframe, Scalar(0, s[i][0], v[i][0]), Scalar(max_H, s[i][1], v[i][1]), mask2);
      mask = mask1 | mask2;
    }
    frame_gray=mask;
    sprintf(chaine,"%d_%s_mask.jpg",i,names[i]);
    imwrite(chaine,frame_gray);
    GaussianBlur( frame_gray, frame_gray, Size(param.k_size_gauss, param.k_size_gauss), param.sigma_gauss, param.sigma_gauss );
    sprintf(chaine,"%d_%s_gauss.jpg",i,names[i]);
    imwrite(chaine,frame_gray);
    Canny( frame_gray, frame_gray, param.lowThreshold_canny, param.lowThreshold_canny*param.ratio_canny, param.k_size_canny );
    sprintf(chaine,"%d_%s_canny.jpg",i,names[i]);
    imwrite(chaine,frame_gray);
    result.push_back(frame_gray.clone());
  }
  return result;
}

void print_global_result(vector< vector <square_robotrovision> > global_result)
{
  int n_colors=global_result.size(),n_squares,i,j;
  char names[5][256]={"orange","black","green","yellow","blue"};

  cout<<n_colors<<" colors searched"<<endl<<endl;
  for(i=0;i<n_colors;i++)
  {
    n_squares=global_result[i].size();
    cout<<"squares color "<<i<<" "<<names[i]<<" squares: "<<n_squares<<endl;
    for(j=0;j<n_squares;j++)
    {
      cout<<"center "<<j<<" "<<global_result[i][j].center<<" side "<<global_result[i][j].side_length<<"\npoints\n"<<global_result[i][j].approx<<endl<<endl;
    }
    cout<<endl<<endl;
  }
}
