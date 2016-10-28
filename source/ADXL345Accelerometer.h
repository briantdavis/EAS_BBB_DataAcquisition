//============================================================================
// Name        : ADXL345Accelerometer.h
// Author      : 
// Version     : 
// Copyright   : 
// Description : ADXL345Accelerometer in C++, Ansi-style
//============================================================================
/*
 * ADXL345Accelerometer.cpp
 * Implementation of a class to interface with the Analog Devices ADXL345 3 Axis Accelerometer
 * over the I2C bus
 *
 * Modified from code by Mahendra Gunawardena, Mitisa LLC
 *
 * For use in EagleAeroSport project
 */

#include "eas_daq_pack.h"

#ifndef ADXL345ACCELEROMETER_H_
#define ADXL345ACCELEROMETER_H_

#define ADXL345_SDO_H_ADDR	0x1d
#define ADXL345_SDO_L_ADDR	0x53

#define ADXL345_POWER_CTL		0x2D	// R/W Power saving features control

/* ADXL345_POWER_CTL Bits */
#define ADXL345_PWRCTL_LINK			(1 << 5)
#define ADXL345_PWRCTL_AUTO_SLEEP	(1 << 4)
#define ADXL345_PWRCTL_MEASURE		(1 << 3)
#define ADXL345_PWRCTL_SLEEP		(1 << 2)
#define ADXL345_PWRCTL_WAKEUP(x)	((x) & 0x3)

#define MG_PER_LSB    (3.90)

// #define MAX_BUS 				64

class ADXL345Accelerometer {

private:
	int i2cBus, i2cAddress, i2cHandle;
  uint8_t uniqueID;
	// char namebuf[MAX_BUS];

	// signed int accelerationX, accelerationY, accelerationZ;
	// signed int roll,pitch;
	signed short rawData_X, rawData_Y, rawData_Z;
	int opResult, tenBitAddress;

	float deviceTemperature;

public:
  //
  // Manager Fn
	ADXL345Accelerometer(int, int, uint8_t = UNSET_UID);
	int initAccelerometer();
 	virtual ~ADXL345Accelerometer();
 
  //
  // Mutator Fn
	void SetPowerMode(unsigned char );
	int updateAccelData();
     
  //
  // Accessor Functions
	unsigned int getAccelerometer_ID();
	signed int getAcceleration_X();
	signed int getAcceleration_Y();
	signed int getAcceleration_Z();
  float getX_float();
  float getY_float();
  float getZ_float(); 
   
  //
  // Utility Fn

  EasDAQpack* fillEASpack(EasDAQpack &);
  int logPartASensorID(std::ofstream &, std::string);
  
};

#endif /* ADXL345ACCELEROMETER_H_ */
