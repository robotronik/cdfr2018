#ifndef FSM_MASTER_H
#define FSM_MASTER_H

typedef enum FSM_Status_E{
  FSM_RUNNING,
  FSM_SUCCESS,
  FSM_ERROR
}FSM_Status;

struct FSM_Instance_S;
typedef void (*FSM_Procedure)(struct FSM_Instance_S*);

typedef struct FSM_Instance_S{
  FSM_Procedure run;
  FSM_Status status;
}FSM_Instance;

void FSM_NOP(FSM_Instance *fsm);

#endif
