//============================================================================
// Name        : SLM9DS1IMU.h
// Author      : Collin Hudson
// Version     : 0.1
// Date        : 3/23/2016
// Description : LSM9DS1 IMU .h file
//============================================================================
/*
 *
 * For use in EagleAeroSport project
 */

// From: https://github.com/sparkfun/SparkFun_LSM9DS1_Arduino_Library/blob/master/src/SparkFunLSM9DS1.h
// With reference to: https://github.com/sparkfun/SparkFun_LSM9DS1_Particle_Library/blob/master/firmware/SparkFunLSM9DS1.h
//                    https://github.com/collinhudson/eas_bbb_version1/blob/master/source/mpu6050AccelGyro.h


#ifndef LSM9DS1_H_
#define LSM9DS1_H_

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

#include "LSM9DS1_Registers.h"
#include "LSM9DS1_Types.h"

#define LSM9DS1_AG_ADDR(sa0)	((sa0) == 0 ? 0x6A : 0x6B)
#define LSM9DS1_M_ADDR(sa1)		((sa1) == 0 ? 0x1C : 0x1E)

enum lsm9ds1_axis {
	X_AXIS,
	Y_AXIS,
	Z_AXIS,
	ALL_AXIS
};

class LSM9DS1
{
public:
	IMUSettings settings;
	
	// We'll store the gyro, accel, and magnetometer readings in a series of
	// public class variables. Each sensor gets three variables -- one for each
	// axis. Call readGyro(), readAccel(), and readMag() first, before using
	// these variables!
	// These values are the RAW signed 16-bit readings from the sensors.
	int16_t gx, gy, gz; // x, y, and z axis readings of the gyroscope
	int16_t ax, ay, az; // x, y, and z axis readings of the accelerometer
	int16_t mx, my, mz; // x, y, and z axis readings of the magnetometer
    int16_t temperature; // Chip temperature
	float gBias[3], aBias[3], mBias[3];
	int16_t gBiasRaw[3], aBiasRaw[3], mBiasRaw[3];
	
	// LSM9DS1 -- LSM9DS1 class constructor
	// The constructor will set up a handful of private variables, and set the
	// communication mode as well.
	// Input:
	//	- interface = Either IMU_MODE_SPI or IMU_MODE_I2C, whichever you're using
	//				to talk to the IC.
	//	- xgAddr = If IMU_MODE_I2C, this is the I2C address of the accel/gyroscope.
	// 				If IMU_MODE_SPI, this is the chip select pin of the gyro (CS_AG)
	//	- mAddr = If IMU_MODE_I2C, this is the I2C address of the magnetometer.
	//				If IMU_MODE_SPI, this is the cs pin of the magnetometer (CS_M)
	LSM9DS1(interface_mode interface, uint8_t xgAddr, uint8_t mAddr);
	LSM9DS1();
		
	// begin() -- Initialize the gyro, accelerometer, and magnetometer.
	// This will set up the scale and output rate of each sensor. The values set
	// in the IMUSettings struct will take effect after calling this function.
	uint16_t begin();
	
	void calibrate(bool autoCalc = true);
	void calibrateMag(bool loadIn = true);
	void magOffset(uint8_t axis, int16_t offset);
	
	// accelAvailable() -- Polls the accelerometer status register to check
	// if new data is available.
	// Output:	1 - New data available
	//			0 - No new data available
	uint8_t accelAvailable();
	
	// gyroAvailable() -- Polls the gyroscope status register to check
	// if new data is available.
	// Output:	1 - New data available
	//			0 - No new data available
	uint8_t gyroAvailable();
	
	// gyroAvailable() -- Polls the temperature status register to check
	// if new data is available.
	// Output:	1 - New data available
	//			0 - No new data available
	uint8_t tempAvailable();
	
	// magAvailable() -- Polls the accelerometer status register to check
	// if new data is available.
	// Input:
	//	- axis can be either X_AXIS, Y_AXIS, Z_AXIS, to check for new data
	//	  on one specific axis. Or ALL_AXIS (default) to check for new data
	//	  on all axes.
	// Output:	1 - New data available
	//			0 - No new data available
	uint8_t magAvailable(lsm9ds1_axis axis = ALL_AXIS);
	
	
	// Source code differs after this point
