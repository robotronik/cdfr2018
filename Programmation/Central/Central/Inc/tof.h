/*
 * The code below is inspired by the STMicroelectronics X-CUBE-53L0A1 example source code.
 * See ../en.SLA0044.txt for the corresponding licence.
 */
#ifndef TOF_H
#define TOF_H

#include "vl53l0x_api.h"

#define TOF_DEFAULT_ADDR 0x52 //Fixed in hardware
#define TOF_DELAY 10

//Shortcut redefinitions
typedef VL53L0X_RangingMeasurementData_t ToF_Data;
typedef VL53L0X_Dev_t ToF_Dev;

typedef struct ToF_Handler_S{
  ToF_Dev dev;
  ToF_Data data;
}ToF_Handler;

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

void ToF_Init_Struct(ToF_Handler *htof, I2C_HandleTypeDef *hi2c);
/**
 * Init the ToF_Handler structure with the given parameters.
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

int ToF_Configure_Device(ToF_Dev *dev, ToF_Params *params, VL53L0X_DeviceModes mode);
/**
 * Configure the device with the given parameters and perform
 * calibration.
 */

int ToF_Perform_Measurement(ToF_Dev *dev, ToF_Data *data);
/**
 * Performs a measurement on the device 'dev' and stores the result in
 * 'data'. This functions blocks until the measurement is completed.
 */

int ToF_Poll_Measurement_Data(ToF_Handler *htof);
/**
 * Poll the device for new data. Returns -1 on error, 1 if there is no
 * new data, and 0 of there is new data.
 */

int ToF_Get_Last_Range(ToF_Handler *htof);
/**
 * Return the last measurement in mm if an object was detected, -1
 * otherwise.
 */

#define TOF_LONG_RANGE_CONFIG(params) {			\
  params.signalLimit = (FixPoint1616_t)(0.25*65536);	\
  params.sigmaLimit = (FixPoint1616_t)(18*65536);	\
  params.timingBudget = 33000;				\
  params.preRangeVcselPeriod = 14;			\
  params.finalRangeVcselPeriod = 10;			\
  }

#define TOF_HIGH_ACCURACY_CONFIG(params) {		\
    params.signalLimit = (FixPoint1616_t)(0.25*65536);	\
    params.sigmaLimit = (FixPoint1616_t)(18*65536);	\
    params.timingBudget = 200000;			\
    params.preRangeVcselPeriod = 14;			\
    params.finalRangeVcselPeriod = 10;			\
  }

#define TOF_HIGH_SPEED_CONFIG(params) {					\
    params.signalLimit = (FixPoint1616_t)(0.25*65536);			\
    params.sigmaLimit = (FixPoint1616_t)(32*65536);			\
    params.timingBudget = 20000;					\
    params.preRangeVcselPeriod = 14;					\
    params.finalRangeVcselPeriod = 10;					\
  }

#endif
