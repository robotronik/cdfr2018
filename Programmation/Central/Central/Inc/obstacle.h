#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <stdint.h>
#include <stdbool.h>
#include "game_defs.h"

#define N_MAX_OBSTACLES 4
#define OBSTACLE_LIFETIME 5000 //ticks

#define SENSOR_DIST_TANGENT 110 //TO ADAPT
#define SENSOR_DIST_PERP 110 //TO ADAPT
#define OBS_RADIUS 150 //TO ADAPT
#define OBS_NODETECT_COUNT 5

#define MARGIN_MAX 150
#define MARGIN_MIN (1.415*SQUARE_SIZE)
/**
 * The MARGIN_MIN constant must be > sqrt(2)*SQUARE_SIZE to guarantee
 * that as long as a rotation can be made safely in a zone that is not
 * an obstacle, a path can be computed from this point.
 */

//Sensors
typedef enum Sensor_E{
  FRONT_LEFT,
  FRONT_RIGHT,
  REAR_LEFT,
  REAR_RIGHT
}Sensor;

//Robots
typedef enum Obstacle_Range_E{
  IN_RANGE_FORWARD,
  IN_RANGE_BACKWARD,
  OUT_OF_RANGE,
}Obstacle_Range;

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
  Obstacle_Range range;
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

int Can_Rotate();
int Can_Move(float distance, bool forward, float *max_speed_ratio);
/**
 * Return true or false wether or not the robot can move 'dist'
 * forward or backward (depending on 'forward' value), assuming that
 * the robot is already aligned in the right direction. If the answer
 * is true, max_speed_ratio is adjusted to limit the speed of the
 * robot, considering the distance of the closest obstacle in range.
 */

void Get_Avoidance_Flexibility(float *fwd_dist, float *bwd_dist);

#endif
