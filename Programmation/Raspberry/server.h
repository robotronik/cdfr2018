#ifndef __SERVER_H__
#define __SERVER_H__

#include "uart/remote_call.h"
#include "client-api/client_api.h"
#include "main.h"

typedef enum Server_Functions_E{
  GAS,
  RAGEQUIT,
  WASTED,
  SO_POINTS_MUCH_SCORE,
  READ_THIS_DAMN_PLAN_MORRIS,
  RANDOM_STUFF,
  GET_ASSER_DATA,
}Server_Functions;

int load_pid_data();

void gas(RC_Server *server);
void ragequit(RC_Server *server);
void wasted(RC_Server *server);
void so_points_much_score(RC_Server *server);
void read_this_damn_plan_morris(RC_Server *server);
void random_stuff(RC_Server *server);
void get_asser_data(RC_Server *server);

#endif
