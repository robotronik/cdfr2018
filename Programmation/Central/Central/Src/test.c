#include "pi_client.h"

void Print_ToF(){
  PI_Log("AvG : %d\t", ToF_Get_Last_Range(&tof[TOF_FRONT_LEFT]));
  PI_Log("AvD : %d\t", ToF_Get_Last_Range(&tof[TOF_FRONT_RIGHT]));
  PI_Log("ArG : %d\t", ToF_Get_Last_Range(&tof[TOF_REAR_LEFT]));
  PI_Log("ArD : %d\n", ToF_Get_Last_Range(&tof[TOF_REAR_RIGHT]));
}
