//
// EagleAeroSport
//
// B.Davis
//
//////////////////


#include <string>
#include <cstdint>

//
// Conventional Guards
#ifndef EAS_PACK_H_
#define EAS_PACK_H_

//
// Constants
//
#define EAS_STR_LEN      24

//
// easDAQpack definition
//
// For EASE of binary store/load
// Only ANSI C (intrinsic) types used in this class/struct
// This could certainly be optimized later
//
class EasDAQpack {

friend std::ostream &operator << (std::ostream &os,
			 const EasDAQpack &e);

public:
//
// Packet IDentifiers
//                              
typedef enum easDAQpackId_e { UNDEF,
                            TIMESTAMP,  
                            ACCEL,
                            BAROTEMP,
                            GYRO,
                            STRAINGAUGE,
                            CLOCK_T,
                            MPU6050_t,
                            ADXL345_t,
                            BMP180_t,
                            BME280_t,
                            HSC_PRESS_t,
                            DUAL_CLOCK_t,
                            BEYOND_DEV } 
easDAQpackId_t;

typedef union easDAQpackData_u {
    clock_t clk_t;
    struct {
      // unsigned long test;
      clock_t course;
      struct timespec ns;
    } time;
    struct {
      int16_t  accel_x, accel_y, accel_z;
      int16_t  gyro_x, gyro_y, gyro_z;
      int16_t  temp;
    } accelGyro;
    struct {
      uint32_t press;
      uint16_t temp;
    } barotemp;
    struct {
      uint32_t Press, Temp;
      uint16_t Humid;
    } PresTempHum;
    struct {
      uint32_t x, y, z;
    } threeAxis_ui32;
    struct {
      signed short x, y, z;
    } threeAxis_ss16;
    struct {
      uint8_t status;             // 2 bit
      uint16_t bridge_data;       // 14 bit
      uint16_t temperature_data;  // 11 bit
    } hsc_press;
    struct {
      uint32_t adc_val;
    } sg;
  } easDAQpackData_t;

static std::string const EAS_PACKET_STRINGS[];

//
// Display Types
//
typedef enum easDAQ_displaymode_e { RAW,
                                  SI,
                                  BASIC,
                                  NONE }
easDAQ_dispmode_t;

static easDAQ_dispmode_t active_dispmode;      

//
// Class Member Data
//
private:
  easDAQpackId_t pack_id;
  //
  // Try to Target 12 byte Max for Union Size
  // for total object/struct size of 16
  //
public: // TODO - change access modifier
  easDAQpackData_t u;
     
  //
  // Class Member Functions
  
  //
  // Manager Fn
  EasDAQpack();
  
  //
  // Mutator Fn
  void blank();
  int setID(easDAQpackId_t id);
  int setClockT(clock_t clk);
  int setClockDual(clock_t, struct timespec);

  //
  // Utility Fn
  int outToFile(std::ofstream &);
  int outToConsole();
                          
}; // class

std::ostream &operator << (std::ostream &os,
			 const EasDAQpack &e);

#endif
