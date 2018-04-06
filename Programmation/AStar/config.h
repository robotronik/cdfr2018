#ifndef CONFIG_H
#define CONFIG_H

//MAP
#define SQUARE_SIZE 20//2cm
#define AREA_HEIGHT 2000
#define AREA_WIDTH 3000
#define MAP_HEIGHT (AREA_HEIGHT / SQUARE_SIZE)
#define MAP_WIDTH (AREA_WIDTH / SQUARE_SIZE)
#define NB_CUBE_SETS 6
#define NB_CUBES (5*NB_CUBE_SETS)
#define CUBE_SIZE 58
#define ROBOT_RADIUS 167//167
#define PADDING 1.2

//HEAP
#define HEAP_SIZE 100000

#endif
