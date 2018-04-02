/*
 * Library for Cdfr2018 @Robotronik
 * You may freely rage with this software
 */

#ifndef __Robotronik_corp_pid_H
#define __Robotronik_corp_pid_H

typedef struct{
  float Kp;
  float Ki;
  float Kd;
  float prev_eps;
  float integral;
  float Te;
  int position_tolerance;
  float speed_tolerance;
} PID_DATA;

void pid_init(volatile PID_DATA *pid);
float pid(volatile PID_DATA *pid, int eps);
int reached(volatile PID_DATA *pid,int eps);

#endif
