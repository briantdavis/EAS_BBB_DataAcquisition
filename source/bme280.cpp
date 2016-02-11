/*
BMP180 Pressure & Temp Sensor Library

bmp180.cpp

EAS

Brian Davis

*/
#include <errno.h>
#include <unistd.h>

#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include <iostream>
#include <cstring>

#include "bme280.h"

//
// Constructor
BME280::BME280(int bus, int addr) {
  #define MAX_BUS 40
  char namebuf[MAX_BUS];
  //-------------------
  blank();
  
  i2cBus = bus;
	snprintf(namebuf, sizeof(namebuf), "/dev/i2c-%d", i2cBus);

	//Create a file descriptor for the I2C bus
	if ((i2cHandle = open(namebuf, O_RDWR)) <= 0) {
		std::cout << "Failed to open BMP280 Sensor on " << namebuf << " I2C Bus" << std::endl;
    std::cout << "Error # = " << ((int)i2cHandle) << std::endl;
    return;
	}

  i2cAddress = addr;
	// Tell the I2C peripheral device address of the accelerometer
	if((opResult = ioctl(i2cHandle, I2C_SLAVE, i2cAddress)) < 0) {
		std::cout << "I2C_SLAVE address " << i2cAddress << " failed..." << std::endl;
		return;
	}
 
 init_device();

} // constructor


//---------------------------------------------
int BME280 :: init_device() {
  // unsigned char rxBuffer[16];		// receive buffer
	unsigned char txBuffer[4];		// transmit buffer
  // unsigned char reg_val;
  //-------------
  
  txBuffer[0] = regCtrlHum;
  //
  // Page 26 of Bosch Datasheet
  //
  txBuffer[1] = 0x02;
  
  // Write
  if ((opResult = write(i2cHandle, txBuffer, 2)) != 2) {
	  std::cout <<"BME280 No ACK bit Write in init!" << std::endl;
    return -1;
  }
  
  txBuffer[0] = regCtrlMeas;
  //
  // Page 27 of Bosch Datasheet
  //
  txBuffer[1] = 0x4B;
  
  // Write
  if ((opResult = write(i2cHandle, txBuffer, 2)) != 2) {
	  std::cout <<"BME280 No ACK bit Write in init!" << std::endl;
    return -1;
  }
  

  
  return 0;
}

//---------------------------------------------
//
// Blank
void BME280 :: blank() {
    memset(this, 0, sizeof(BME280));
}

//----------------------------------------------
//
// Destructor
//
BME280::~BME280() {
	// TODO Auto-generated destructor stub
  close(i2cHandle);
}

int BME280 :: updateHumidPressTemp() {
	unsigned char rxBuffer[16];		// receive buffer
	unsigned char txBuffer[1];		// transmit buffer

	txBuffer[0] = BME280_RA_PRESS_MSB ;  // This is the address we want to read from.
 
	if ((opResult = write(i2cHandle, txBuffer, 1)) != 1) {
	  std::cout <<"BME280 No ACK bit Write!" << std::endl;
    return -1;
  } 
  
  if((opResult = read(i2cHandle, rxBuffer, 8)) != 8) {
		std::cout <<"BME280 No ACK bit Read!" << std::endl;
    return -1;
	}

  rawPress = (rxBuffer[0] << 12) | (rxBuffer[1] << 4) | (rxBuffer[2] >> 4);
  rawTemp = (rxBuffer[3] << 12) | (rxBuffer[4] << 4) | (rxBuffer[5] >> 4);
  rawHumid = (rxBuffer[6] << 8) | (rxBuffer[7]);

	return (0);
 }
 
  //
// Eagle Aero Sport Extensions
//
EasDAQpack* BME280::fillEASpack(EasDAQpack &fillPack)
{
  fillPack.setID(EasDAQpack::BME280_t);
  fillPack.u.PresTempHum.Press = rawPress;
  fillPack.u.PresTempHum.Temp = rawTemp;
  fillPack.u.PresTempHum.Humid = rawHumid;
  
  return &fillPack;
}

