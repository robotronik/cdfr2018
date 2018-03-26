#include "tof.h"

void ToF_Init_Struct(ToF_Dev *dev, I2C_HandleTypeDef *hi2c, uint8_t addr){
  dev->I2cHandle = hi2c;
  dev->I2cDevAddr = addr;
  dev->Present = 0;

  //Set I2C speed to 400KHz
  VL53L0X_WrByte(dev, 0x88, 0x00);
}

int ToF_Poke(ToF_Dev *dev){
  int status;
  //Read ID to know if the address is correct
  uint16_t id;
  status = VL53L0X_RdWord(dev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &id);
  
  //I2C error or bad ID
  if(status || id != 0xEEAA){
    return -1;
  }

  return 0;
}

int ToF_Set_Address(ToF_Dev *dev, uint8_t addr){
  //Set device address
  int status = VL53L0X_SetDeviceAddress(dev, addr);
  if(status) return -1;
  dev->I2cDevAddr = addr;

  //Check if the device work with the new address
  return ToF_Poke(dev);
}

int ToF_Init_Device(ToF_Dev *dev){
  //Init the device
  int status = VL53L0X_DataInit(dev);
  if(status == 0)
    dev->Present = 1;
  else
    return -1;
  
  //Static init
  status = VL53L0X_StaticInit(dev);
  if(status) return -1;
  return 0;
}

int ToF_Configure_Device(ToF_Dev *dev, ToF_Params *params){
  //Ref calibration
  VL53L0X_PerformRefCalibration(dev, &params->VhvSettings, &params->PhaseCal);

  //Ref Spad Management
  VL53L0X_PerformRefSpadManagement(dev, &params->refSpadCount, &params->isApertureSpads);

  //Set single ranging mode
  VL53L0X_SetDeviceMode(dev, VL53L0X_DEVICEMODE_SINGLE_RANGING);

  //Enable Sigma Limit
  VL53L0X_SetLimitCheckEnable(dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);

  //Enable Signal Limit
  VL53L0X_SetLimitCheckEnable(dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);

  VL53L0X_SetLimitCheckValue(dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, params->signalLimit);
  VL53L0X_SetLimitCheckValue(dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, params->sigmaLimit);
  VL53L0X_SetMeasurementTimingBudgetMicroSeconds(dev, params->timingBudget);
  VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, params->preRangeVcselPeriod);
  VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, params->finalRangeVcselPeriod);
  VL53L0X_PerformRefCalibration(dev, &params->VhvSettings, &params->PhaseCal);

  dev->LeakyFirst = 1;

  return 0;
}

int ToF_Perform_Measurement(ToF_Dev *dev, ToF_Data *data){
  static int LeakyFactorFix8 = (int)( 0.6 *256);

  if(!dev->Present) return -1;

  VL53L0X_PerformSingleRangingMeasurement(dev, data);

  //Sensor_SetNewRange
  /* Store new ranging data into the device structure, apply leaky
     integrator if needed */
  if(data->RangeStatus == 0){
    if(dev->LeakyFirst){
      dev->LeakyFirst = 0;
      dev->LeakyRange = data->RangeMilliMeter;
    }
    else{
      dev->LeakyRange = (dev->LeakyRange*LeakyFactorFix8 + (256-LeakyFactorFix8)*data->RangeMilliMeter)>>8;
    }
  }
  else{
    dev->LeakyFirst = 1;
  }
  
  return 0;
}
