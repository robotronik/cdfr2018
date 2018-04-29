#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <stdint.h>
#include "game_defs.h"

#define N_MAX_OBSTACLES 4
#define OBSTACLE_LIFETIME 5000 //ticks

#define SENSOR_DIST_TANGENT 40 //TO ADAPT
#define SENSOR_DIST_PERP 60 //TO ADAPT
#define OBS_RADIUS 150 //TO ADAPT
#define OBS_NODETECT_COUNT 5

#define MARGIN_MAX 100
#define MARGIN_MIN 50

//Sensors
typedef enum Sensor_E{
  FRONT_LEFT,
  FRONT_RIGHT,
  REAR_LEFT,
  REAR_RIGHT
}Sensor;

//Robots
typedef struct Obstacle_S{
  //System ticks when the last detection occured
  uint32_t last_detection;
  //Distance from the center of our robot to the obstacle, in mm
  uint16_t distance;
  //Detected position (real obstacle)
  int16_t x, y;
  //Guessed position (center)
  int16_t x_c, y_c;
  uint16_t distance_c;
  uint8_t no_detect;
}Obstacle;

extern Obstacle obstacle[N_MAX_OBSTACLES];
extern int nb_obstacles;
extern uint16_t sensor_raw[4];

void Update_Obstacles(const Robot *ref, uint16_t fl_d, uint16_t fr_d, uint16_t rl_d, uint16_t rr_d);
/**
 * Params : distance for each sensor, 0 means no detection.
 */

void Print_Obstacles(void);

int Materialize_Obstacle(Obstacle *obs, uint16_t margin);
/**
 * Try to materialize an obstacle for integration in path
 * finding. Returns 0 on success. If it fails (ie. the obstacle is too
 * close), returns -1.
 */

int Materialize_Obstacles(uint16_t margin);

int Is_Too_Close(Obstacle *obs, uint16_t margin);

int Is_In_Range(Obstacle *obs, const Robot *ref);
/**
 * Returns 0 if the obstacle isn't in range, 1 if it's at the front,
 * -1 if it's at the back.
 */

#endif
