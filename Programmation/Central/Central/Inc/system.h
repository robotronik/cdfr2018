#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#include "main.h"
#include "tim.h"
#include "pi_client.h"

#define JACK_FILTER 5

//Team enum
typedef enum Team_E{
  //Just swap these two enums to mirror the select button
  GREEN_TEAM = GPIO_PIN_RESET,
  ORANGE_TEAM = GPIO_PIN_SET
}Team;

Team wait_start();
/**
 * Waits for the match to start. Returns the selected team when the
 * match begins.
 */

void stop();
void hlt();

#endif
