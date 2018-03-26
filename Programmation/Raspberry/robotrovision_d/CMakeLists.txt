cmake_minimum_required(VERSION 2.8)
project( RobotrOvision )
list(APPEND CMAKE_MODULE_PATH "./FindTBB/")

find_package( OpenCV REQUIRED ) 
add_executable( RobotrOvision Code.cpp fonctions.cpp)
target_link_libraries( RobotrOvision ${OpenCV_LIBS}  tbb)
