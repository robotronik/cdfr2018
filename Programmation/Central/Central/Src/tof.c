#include "tof.h"
#include "pi_client.h"

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
  uint16_t id = 0;
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
  int status = 0;

  do{
    //Ref Spad Management
    status = VL53L0X_PerformRefSpadManagement(dev, &params->refSpadCount, &params->isApertureSpads);
    if(status) PI_Log("RefSpad failed : %d\n", status);
    if(status) break;

    //Ref calibration
    //status = VL53L0X_PerformRefCalibration(dev, &params->VhvSettings, &params->PhaseCal);
    if(status) break;
   
    //Set single ranging mode
    status = VL53L0X_SetDeviceMode(dev, VL53L0X_DEVICEMODE_SINGLE_RANGING);
    if(status) break;
  
    //Enable Sigma Limit
    status = VL53L0X_SetLimitCheckEnable(dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
    if(status) break;
  
    //Enable Signal Limit
    status = VL53L0X_SetLimitCheckEnable(dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
    if(status) break;
  
    status = VL53L0X_SetLimitCheckValue(dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, params->signalLimit);
    if(status) break;
  
    status = VL53L0X_SetLimitCheckValue(dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, params->sigmaLimit);
    if(status) break;
  
    status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(dev, params->timingBudget);
    if(status) break;
  
    status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, params->preRangeVcselPeriod);
    if(status) break;

    status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, params->finalRangeVcselPeriod);
    if(status) break;
  
    status = VL53L0X_PerformRefCalibration(dev, &params->VhvSettings, &params->PhaseCal);
    if(status) break;
  
    dev->LeakyFirst = 1;

    return 0;
  }while(0);
  
  return -1;
}

int ToF_Perform_Measurement(ToF_Dev *dev, ToF_Data *data){
  static int LeakyFactorFix8 = (int)( 0.6 *256);

  if(!dev->Present) return -1;

  if(VL53L0X_PerformSingleRangingMeasurement(dev, data) == 0){
    //LED_ON;
  }

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
