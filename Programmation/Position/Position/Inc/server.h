#ifndef __SERVER_H_
#define __SERVER_H_

#include "robotronik_protocol.h"
#include "robotronik_protocol_stm32f3.h"
#include "remote_call.h"
#include "stm32f3xx_it.h"
#include "Robotronik_corp_pid.h"
#include "fsm_master.h"
#include "fsm_position_abs.h"
#include "fsm_position_pts.h"
#include "odometry.h"

typedef enum P_Functions_E{
  P_GET_STATE,
  P_RESET,
  P_SET_ASSER_SUM,
  P_SET_ASSER_DIFF,
  P_SET_N_POINTS_ASSER,
  P_SET_ODO,
  P_GET_ODO,
  P_BALEC,
  P_SET_N_POINTS,
  P_GET_N_POINTS,
  P_GO_FORWARD,
  P_SET_ANGLE,
  P_BRAKE,
  P_SET_POSITION_X_Y
}P_Functions;

typedef enum P_States_E{
  P_READY,
  P_BUSY,
  P_ERROR
}P_States;

void reset(RC_Server* pserver);
void get_state(RC_Server* pserver);
void reset(RC_Server* pserver);
void set_asser_sum(RC_Server* pserver);
void set_asser_diff(RC_Server* pserver);
void balec(RC_Server* pserver);
void set_odo(RC_Server* pserver);
void get_odo(RC_Server* pserver);

void go_forward(RC_Server* pserver);
void set_angle(RC_Server* pserver);

void set_n_points(RC_Server* pserver);//use to idicate the number of interpolation points
void get_n_points(RC_Server* pserver);
void set_n_points_asser(RC_Server* pserver);

void brake(RC_Server* pserver);
void set_position_x_y(RC_Server* pserver);

#endif
