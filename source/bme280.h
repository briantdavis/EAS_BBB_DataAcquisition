//
// EAS
//
// Brian Davis
//
//
// Based from :
//       https://www.adafruit.com/products/2652
//       http://www.embeddedadventures.com/bme280_weather_multi_sensor_mod-1022.html
//       https://github.com/embeddedadventures/BME280
//
//       https://github.com/BoschSensortec/BME280_driver/blob/master/bme280.c
//
//

#ifndef BME280_H_
#define BME280_H_

//
// Includes
//

#include <cstdint>

#include "eas_daq_pack.h"

//
// Sensor Constants
//
#define BME280_SDO_L_ADDR        0x76
#define BME280_SDO_H_ADDR        0x77

// Register Addresses
#define BME280_RA_PRESS_MSB      0xF7

// register defines

#define regCalibStart            0x88
#define regChipID                0xd0
#define regReset          0xe0
#define regCalibStart2    0xe1
#define regCtrlHum        0xf2
#define regStatus         0xf3
#define regCtrlMeas       0xf4
#define regConfig         0xf5
#define regMeasurementsStart      0xf7

//
// Sensor Class definition
//
class BME280 {

// t_sb standby options - effectively the gap between automatic measurements 
// when in "normal" mode

enum standbySettings_e {
  tsb_0p5ms,
  tsb_62p5ms,
  tsb_125ms,
  tsb_250ms,
  tsb_500ms,
  tsb_1000ms,
  tsb_10ms,
  tsb_20ms
};

// sensor modes, it starts off in sleep mode on power on
// forced is to take a single measurement now
// normal takes measurements reqularly automatically

enum mode_e {
  smSleep,
  smForced,
  smNormal = 3,
};

// Filter coefficients
// higher numbers slow down changes, such as slamming doors

enum filterCoefficient_e {
  fc_off,
  fc_2,
  fc_4,
  fc_8,
  fc_16
};

// Oversampling options
// Oversampling reduces the noise from the sensor

enum oversampling_e {
  osSkipped,
  os1x,
  os2x,
  os4x,
  os8x,
  os16x,
};

// Structure to hold the compensation parameters necessary for the calculations

typedef struct _compParams_ts {
  uint16_t dig_T1;
  int16_t  dig_T2;
  int16_t  dig_T3;

  uint16_t dig_P1;
  int16_t  dig_P2;
  int16_t  dig_P3;
  int16_t  dig_P4;
  int16_t  dig_P5;  
  int16_t  dig_P6;  
  int16_t  dig_P7; 
  int16_t  dig_P8;
  int16_t  dig_P9;

  uint8_t  dig_H1;
  int16_t  dig_H2;
  uint8_t  dig_H3;
  int16_t  dig_H4;
  int16_t  dig_H5;
  uint8_t  dig_H6;
  
} compParams_ts;

union compParams_u
{
   uint8_t        compArray[24+9];
   compParams_ts  compStruct;
};

  //
  // Class member data
  //

  private:
    int i2cBus;       // processor bus #
    int i2cAddress;   // i2c bus addres (7-bit)
    int i2cHandle;    // Handle returned from open()
    uint8_t uniqueID;
    int opResult;
    
    uint32_t rawPress;
    uint32_t rawTemp;
    uint16_t rawHumid;
    
  //
  // Mutator
  void blank();
  
    public:
    BME280(int, int, uint8_t = 0xFF);
    virtual ~BME280();
    
    int init_device();
    int updateHumidPressTemp();
    
  //
  // Utility Fn
  EasDAQpack* fillEASpack(EasDAQpack &);
  int logPartASensorID(std::ofstream &, std::string);
  int logPartBSensorCal(std::ofstream & ofile);                      
                  
}; // end class


#endif