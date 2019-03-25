//
// EAS
//
// Brian Davis
//
//
// Based from https://github.com/rodan/honeywell_hsc_ssc_i2c

#ifndef HSC_PRESS_H_
#define HSC_PRESS_H_

//
// Includes
//

#include <fcntl.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include <iostream>
#include <cstring>

#include "eas_daq_pack.h"

// #include "hscPress.h"

#define HSC_PN2_ADDR  0x28
#define HSC_PN3_ADDR  0x38
#define HSC_PN4_ADDR  0x48
#define HSC_PN5_ADDR  0x58
#define HSC_PN6_ADDR  0x68
#define HSC_PN7_ADDR  0x78

class HscPress {
  private:
    int i2cBus;       // processor bus #
    int i2cAddress;   // i2c bus addres (7-bit)
    int i2cHandle;    // Handle returned from open()
    uint8_t uniqueID;
    int opResult;
  
    uint8_t status;             // 2 bit
    uint16_t bridge_data;       // 14 bit
    uint16_t temperature_data;  // 11 bit
    
  //
  // Mutator
  void blank();
 

/// function that requests raw data from the sensor via i2c
///
/// input
///  slave_addr    - i2c slave addr of the sensor chip
///
/// output
///  raw           - struct containing 4 bytes of read data
///
/// returns
///         0 if all is fine
///         1 if chip is in command mode
///         2 if old data is being read
///         3 if a diagnostic fault is triggered in the chip
///         4 if the sensor is not hooked up
// uint8_t ps_get_raw(const uint8_t slave_addr, struct cs_raw *raw);

/// function that converts raw data read from the sensor into temperature and pressure values
///
/// input:
///  raw            - struct containing all 4 bytes read from the sensor
///  output_min     - output at minimal calibrated pressure (counts)
///  output_max     - output at maximum calibrated pressure (counts)
///  pressure_min   - minimal value of pressure range
///  pressure_max   - maxium value of pressure range
///
/// output:
///  pressure
///  temperature
// uint8_t ps_convert(const struct cs_raw raw, float *pressure, float *temperature,
//                   const uint16_t output_min, const uint16_t output_max, const float pressure_min,
//                   const float pressure_max);
  public:
    HscPress(int, int, uint8_t = 0xFF);
    virtual ~HscPress();
    
    int init_device();
    int updatePressTemp();
    uint8_t getStatus();
    uint16_t getBridgeData();
    uint16_t getTemperatureData();
    
  //
  // Utility Fn
  EasDAQpack* fillEASpack(EasDAQpack &);
  int logPartASensorID(std::ofstream &, std::string);                      
                  
}; // close class

#endif