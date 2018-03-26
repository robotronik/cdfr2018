#ifndef __TOF_H_
#define __TOF_H_

#include "vl53l0x_api.h"

//Shortcut redefinitions
typedef VL53L0X_RangingMeasurementData_t ToF_Data;
typedef VL53L0X_Dev_t ToF_Dev;

typedef struct ToF_Params_S{
  //Read
  uint8_t VhvSettings;
  uint8_t PhaseCal;
  uint32_t refSpadCount;
  uint8_t isApertureSpads;

  //Write
  FixPoint1616_t signalLimit;
  FixPoint1616_t sigmaLimit;
  uint32_t timingBudget;
  uint8_t preRangeVcselPeriod;
  uint8_t finalRangeVcselPeriod;
}ToF_Params;

void ToF_Init_Struct(ToF_Dev *dev, I2C_HandleTypeDef *hi2c, uint8_t addr);
/**
 * Init the ToF_Dev structure with the given parameters.
 */

int ToF_Poke(ToF_Dev *dev);
/**
 * Check wether or not the given ToF sensor is present or not. Returns
 * 0 if the sensor responds, -1 otherwise.
 */

int ToF_Set_Address(ToF_Dev *dev, uint8_t addr);
/**
 * Set a new address to the device and returns 0 on success, -1 on
 * error.
 */

int ToF_Init_Device(ToF_Dev *dev);
/**
 * Performs basic device initialization.
 */

int ToF_Configure_Device(ToF_Dev *dev, ToF_Params *params);
/**
 * Configure the device with the given parameters and perform
 * calibration.
 */

int ToF_Perform_Measurement(ToF_Dev *dev, ToF_Data *data);

#define TOF_LONG_RANGE_CONFIG(params) {			\
  params.signalLimit = (FixPoint1616_t)(0.25*65536);	\
  params.sigmaLimit = (FixPoint1616_t)(18*65536);	\
  params.timingBudget = 33000;				\
  params.preRangeVcselPeriod = 14;			\
  params.finalRangeVcselPeriod = 10;			\
  }							\

#endif
