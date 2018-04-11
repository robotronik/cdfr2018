/*
 * The code below is inspired by the STMicroelectronics X-CUBE-53L0A1 example source code. Here is the license disclaimer :
 * 
 * COPYRIGHT(c) 2015 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
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
