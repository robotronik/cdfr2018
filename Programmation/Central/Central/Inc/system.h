#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#include "main.h"
#include "tim.h"
#include "pi_client.h"
#include "strategy.h"

#define JACK_FILTER 5

void wait_start();
/**
 * Waits for the match to start. Returns the selected team when the
 * match begins.
 */

void stop();
void remote_stop();
void hlt();
void Print_ToF();

#endif
