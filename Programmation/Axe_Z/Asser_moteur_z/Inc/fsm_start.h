#ifndef FSM_START_H
#define FSM_START_H

#include "fsm_master.h"
#include "Z_axis.h"

typedef struct FSM_Start_S{
  FSM_Instance instance;
}FSM_Start;

void FSM_Start_Init(FSM_Instance *fsm);

void FSM_START_WAIT_AX(FSM_Instance *fsm);
/**
 * Waits until the servos are ready.
 */

void FSM_START_ACTIVATE(FSM_Instance *fsm);
/**
 * Locks the pliers, folds the arm and set the Z axis to down
 * position.
 */

void FSM_START_WAIT_GOAL(FSM_Instance *fsm);
/**
 * Waits until the Z goal is reached.
 */

#endif
