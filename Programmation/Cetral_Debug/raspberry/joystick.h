#ifndef JOYSTICK_H
#define JOYSTICK_H

typedef enum DS3_Axis_E{
  DS3_LEFT_AXIS_X = 0,
  DS3_LEFT_AXIS_Y = 1,
  DS3_RIGHT_AXIS_X = 2,
  DS3_RIGHT_AXIS_Y = 3,
  DS3_UP_ARROW_AXIS = 8,
  DS3_RIGHT_ARROW_AXIS = 9,
  DS3_DOWN_ARROW_AXIS = 10,
  //DS3_LEFT_ARROW_AXIS = 11,
  DS3_L2_AXIS = 12,
  DS3_R2_AXIS = 13,
  DS3_L1_AXIS = 14,
  DS3_R1_AXIS = 15,
  DS3_TRIANGLE_AXIS = 16,
  DS3_CIRCLE_AXIS = 17,
  DS3_CROSS_AXIS = 18,
  DS3_SQUARE_AXIS = 19
}DS3_Axis;

typedef enum DS3_Button_E{
  DS3_SELECT = 0,
  DS3_LEFT_AXIS_BTN = 1,
  DS3_RIGHT_AXIS_BTN = 2,
  DS3_START = 3,
  DS3_UP = 4,
  DS3_RIGHT = 5,
  DS3_DOWN = 6,
  DS3_LEFT = 7,
  DS3_L2 = 8,
  DS3_R2 = 9,
  DS3_L1 = 10,
  DS3_R1 = 11,
  DS3_TRIANGLE = 12,
  DS3_CIRCLE = 13,
  DS3_CROSS = 14,
  DS3_SQUARE = 15,
  DS3_PS = 16
}DS3_Button;

#endif