/* inspired by https://techthetroll.files.wordpress.com/2016/07/trajectoire_courbe.pdf
from Pierre-François GIMENEZ pierre-francois.gimenez@irit.fr
*/

#ifndef FSM_POSITION_PTS_H
#define FSM_POSITION_PTS_H

#include "fsm_master.h"
#include "Robotronik_corp_pid.h"
#include "interpol.h"

typedef struct FSM_Position_Pts_S{
  FSM_Instance instance;
  Interpol points;
  int reception;//number of recieved points
  float w;//pulsation
  float z;//amortissement
  float vc;//maximum speed
  float vr;//rotation speed
  PID_SPEED_DATA pid_speed_l;
  PID_SPEED_DATA pid_speed_r;
  float speed_percent_tolerance;
}FSM_Position_Pts;

void FSM_Pts_Run(FSM_Instance*);

#endif
