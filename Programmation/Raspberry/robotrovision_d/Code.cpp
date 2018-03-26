#include "opencv2/opencv.hpp"
#include <iostream>

#include "fonctions.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "tbb/tbb.h"

#define CV_SOCKET "./cv_socket"

using namespace std;
using namespace cv;
using namespace tbb;

class ComputeSquares{
private:
  vector< vector<int> > *const m_h;
  vector< vector<int> > *const m_s;
  vector< vector<int> > *const m_v;
  PARAM_HSV *const m_param_hsv;
  Mat *const m_image;
  PARAM *const m_param;
  vector <square_robotrovision> *const m_global_result;
  
public:

  ComputeSquares(vector< vector<int> > *const h,
		 vector< vector<int> > *const s,
		 vector< vector<int> > *const v,
		 PARAM_HSV *const param_hsv,
		 Mat *const image,
		 PARAM *const param,
		 vector <square_robotrovision> *global_result) :
    m_h(h), m_s(s), m_v(v), m_param_hsv(param_hsv), m_image(image), m_param(param), m_global_result(global_result) {}

  void operator()(const blocked_range<size_t>& r) const {
    for(size_t i = r.begin(); i != r.end(); ++i){
      m_global_result[i] = find_squares(separate_color(*m_image, (*m_h)[i], (*m_s)[i], (*m_v)[i], *m_param_hsv), *m_param);
    }
  }
};

int main(int argc, char *argv[])
{
  int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if(socket_fd == -1){
    perror("socket");
  }

  struct sockaddr_un sockaddr;
  sockaddr.sun_family = AF_UNIX;
  strncpy(sockaddr.sun_path, CV_SOCKET, sizeof(CV_SOCKET));

  socklen_t addrlen = sizeof(sockaddr);
  if(connect(socket_fd, (const struct sockaddr*) (&sockaddr), addrlen) == -1){
    perror("connect");
  }

  int size;
  read(socket_fd, &size, sizeof(size));
  printf("Image size : %d\n", size);
  unsigned char *raw_img = (unsigned char*) calloc(size, sizeof(*raw_img));
  unsigned char *raw_p = raw_img;
  int count;
  int left = size;
  while((count = read(socket_fd, raw_p, size)) > 0 && (left -= count)){
    raw_p += count;
  }
  if(left == 0){
    printf("Raw image received\n");
  }else{
    printf("WARNING : Image partially received\n");
  }

  task_scheduler_init init;
  Mat image,gray,thresh;
  vector< square_robotrovision> result;
  vector <square_robotrovision> global_result[5];
  vector< Mat > color_separation;

  PARAM param;
  PARAM_HSV param_hsv;

  char pattern[10];
  //names[5][256]={"orange","black","green","yellow","blue"}

  vector< vector<int> > h(5, vector<int>(2));
  vector< vector<int> > s(5, vector<int>(2));
  vector< vector<int> > v(5, vector<int>(2));

  int n=h.size();

  if(load_args("robotrovision_d/config.txt", &param, &param_hsv, h, s, v) == -1){
    cerr << "Can't load parameters !" << endl;
  }

  Mat raw(1, size, CV_8UC1, (void*)raw_img);
  image = imdecode(raw, CV_LOAD_IMAGE_COLOR);

  //image = imread("capture.jpg", -1);
  if(image.rows==0)
  {
    cout<<"Invalid input file"<<endl;
    return 0;
  }

  
  parallel_for(blocked_range<size_t>(0,n), ComputeSquares(&h, &s, &v, &param_hsv, &image, &param, global_result));

  //print_global_result(global_result);

  //for(i=0;i<result[0].approx.size();i++) line(image, result[0].approx[i], result[0].approx[(i+1)%result[0].approx.size()], Scalar(0, 0, 255), 1, 8, 0);
  //imshow("image",image);
  //waitKey(0);

  int r = find_pattern(global_result, pattern);
  if(!r){
    pattern[0] = pattern[1] = pattern[2] = '-';
  }
  pattern[3] = '\0';

  if(write(socket_fd, pattern, 4) == -1){
    perror("write");
  }

  cout << "Image processed. ";
  if(r){
    cout << "Found " << pattern << " pattern" << endl;
  }else{
    cout << "No pattern found." << endl;
  }
  
  close(socket_fd);
  return 0;
}
