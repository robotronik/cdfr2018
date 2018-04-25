#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <stdint.h>
#include "game_defs.h"

#define N_MAX_OBSTACLES 4
#define OBSTACLE_LIFETIME 5000 //ticks

#define SENSOR_DIST_TANGENT 40 //TO ADAPT
#define SENSOR_DIST_PERP 60 //TO ADAPT
#define OBS_RADIUS 150 //TO ADAPT

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
}Obstacle;

extern Obstacle obstacle[N_MAX_OBSTACLES];
extern int nb_obstacles;

void Update_Obstacles(const Robot *ref, uint16_t fl_d, uint16_t fr_d, uint16_t rl_d, uint16_t rr_d);
/**
 * Params : distance for each sensor, 0 means no detection.
 */

void Print_Obstacles(void);

#endif
