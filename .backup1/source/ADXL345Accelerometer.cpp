//============================================================================
// Name        : 
// Author      : 
// Version     : 
// Copyright   : Your copyright notice
// Description : ADXL345Accelerometer in C++, Ansi-style
//============================================================================
/*
 * ADXL345Accelerometer.cpp
 *
 * Implementation of a class to interface with the Analog Devices ADXL345 3 Axis Accelerometer
 * over the I2C bus
 *
 * Based on software from Mahendra Gunawardena, Mitisa LLC
 *
 */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stropts.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

#include "ADXL345Accelerometer.h"

using namespace std;

//---------------------------------------------------------------------
//
//
ADXL345Accelerometer::ADXL345Accelerometer(int busID, int deviceAddress, uint8_t uniqueID_in) {
	i2cBus = busID;
	i2cAddress = deviceAddress;
  uniqueID = uniqueID_in;
	//accelerationX = accelerationY = accelerationZ=0;
	//roll = pitch = 0;
	rawData_X = rawData_Y = rawData_Z = 0;
	deviceTemperature = tenBitAddress = opResult = 0;

	initAccelerometer();
 
   //std::cout << "ADXL345 Num1 Accel Device ID : "<< std::hex << accelNum1.getAccelerometer_ID() << 
   //   std::endl << std::endl;
      
  SetPowerMode(0x01);
  // accelNum1.updateAccelData();
 
}
// -----------
//---------------------------------------------------------------------
//
//
int ADXL345Accelerometer::initAccelerometer()
{
	char namebuf[MAX_BUS];
	snprintf(namebuf, sizeof(namebuf), "/dev/i2c-%d", i2cBus);

	//Create a file descriptor for the I2C bus
	if ((i2cHandle = open(namebuf, O_RDWR))<0) {
		 cout << "Failed to open ADXL345 Sensor on " << namebuf << " I2C Bus" << endl;
		 return (1);
	}

	opResult = ioctl(i2cHandle, I2C_TENBIT, tenBitAddress);
 
	// Tell the I2C peripheral device address of the accelerometer
	if((opResult = ioctl(i2cHandle, I2C_SLAVE, i2cAddress))<0) {
		cout << "I2C_SLAVE address " << i2cAddress << " failed..." << endl;
		return(2);
	}

	return (0);
}
// ------------------
//---------------------------------------------------------------------
//
//
unsigned int ADXL345Accelerometer::getAccelerometer_ID(){
	unsigned char rxBuffer[1];		// receive buffer
	unsigned char txBuffer[1];		// transmit buffer

	txBuffer[0] = 0x00; // This is the address we want to read from.
	if ((opResult = write(i2cHandle, txBuffer, 1))!=1){
		cout <<"ADXL No WR ACK bit!" << endl;
		return (0x01);
	}
	opResult = read(i2cHandle, rxBuffer, 1);
	//cout <<"Part ID: "  << hex << (int) rxBuffer[0]<< endl;
	return ((unsigned int)rxBuffer[0]);
}

// --------------------
//---------------------------------------------------------------------
//
//
void ADXL345Accelerometer::SetPowerMode(unsigned char powerMode)
{
	unsigned char rxBuffer[1];		// receive buffer
	unsigned char txBuffer[2];		// transmit buffer

	unsigned char PowerCtl[1];

	//snprintf(txBuffer, sizeof(txBuffer), "%x", );
	txBuffer[0] = 0x2d ;// (unsigned char) ADXL345_POWER_CTL; // This is the address we want to read from.
	if ((opResult = write(i2cHandle, txBuffer, 1))!=1){
		cout <<"ADXL No ACK WR bit!" << endl;
   return;
	}
	opResult = read(i2cHandle,rxBuffer, 1);
	//cout <<"Part ID: "  << hex << (int) rxBuffer[0]<< endl;

	PowerCtl[0] = rxBuffer[0] & ~ADXL345_PWRCTL_MEASURE;
	txBuffer[1] = PowerCtl[0] | (powerMode * ADXL345_PWRCTL_MEASURE);
	if ((opResult = write(i2cHandle, txBuffer , 2))!=2){
		cout <<"ADXL No ACK WR bit!" << endl;
   return;
	}
}

//---------------------------------------------------------------------
//
//
int ADXL345Accelerometer::updateAccelData()
{
	unsigned char rxBuffer[8];		// receive buffer
	unsigned char txBuffer[1];		// transmit buffer

	txBuffer[0] = 0x32 ;// (unsigned char) ADXL345_POWER_CTL; // This is the address we want to read from.
  
		if ((opResult = write(i2cHandle, txBuffer, 1))!=1){
			cout <<"ADXL No ACK bit Write!" << endl;
      return -1;
		}
   
   if((opResult = read(i2cHandle, rxBuffer, 6))!=6){
			cout <<"ADXL No ACK bit Read!" << endl;
      return -1;
		}

	rawData_X = (rxBuffer[1]<<8)|rxBuffer[0];
	rawData_Y = (rxBuffer[3]<<8)|rxBuffer[2];
	rawData_Z = (rxBuffer[5]<<8)|rxBuffer[4];

	return (0);
}

float ADXL345Accelerometer::getX_float(){
  float x = (rawData_X * MG_PER_LSB) / 1000;
  return x;
}

float ADXL345Accelerometer::getY_float(){
  float y = (rawData_Y * MG_PER_LSB) / 1000;
  return y;
}

float ADXL345Accelerometer::getZ_float(){
  float z = (rawData_Z * MG_PER_LSB) / 1000;
  return z;
}

signed int ADXL345Accelerometer::getAcceleration_X(){
	signed int accelerationX = (signed int)(((signed int)rawData_X) * 3.9);
	return (accelerationX);
}

signed int ADXL345Accelerometer::getAcceleration_Y(){
	signed int accelerationY = (signed int)(((signed int)rawData_Y) * 3.9);
	return (accelerationY);
}

signed int ADXL345Accelerometer::getAcceleration_Z(){
	signed int accelerationZ = (signed int)(((signed int)rawData_Z) * 3.9);
	return (accelerationZ);
}
//
// Destructor
//
ADXL345Accelerometer::~ADXL345Accelerometer() {
	// TODO Auto-generated destructor stub
}

//
// Eagle Aero Sport Extensions
//
EasDAQpack* ADXL345Accelerometer::fillEASpack(EasDAQpack &fillPack)
{
  fillPack.setID(EasDAQpack::ACCEL);
  fillPack.setUnique(uniqueID);
  fillPack.u.threeAxis_ss16.x = rawData_X;
  fillPack.u.threeAxis_ss16.y = rawData_Y;
  fillPack.u.threeAxis_ss16.z = rawData_Z;
  // fillPack.u_sensor_id = uniqueID;
  return &fillPack;
}

// @@@ TODO
// Guards

