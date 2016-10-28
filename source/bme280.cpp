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
#include <fstream>
#include <iomanip>  // setw() setfill()

#include "bme280.h"

//
// Constructor
BME280::BME280(int bus, int addr, uint8_t uniqueID_in) {
  #define MAX_BUS 40
  char namebuf[MAX_BUS];
  //-------------------
  blank();
  
  uniqueID = uniqueID_in;
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
 
 int BME280::logPartASensorID(std::ofstream & ofile, std::string sen_name)
{
  ofile << "Sensor unique ID : " << std::hex << (int)uniqueID << std::endl;
  ofile << "Sensor type : BME280 Temperature Humidity Pressure Sensor" << std::endl;
  ofile << "Sensor bus : /dev/i2c-" << i2cBus << std::endl;
  ofile << "I2C Address : " << (int) i2cAddress << std::endl;
  ofile << "Sensor name / Placement : " << sen_name << std::endl;
  
  logPartBSensorCal(ofile);
  
  ofile << "-----" << std::endl;

  return 0;
}

/*!
 *	@brief This API is used to
 *	calibration parameters used for calculation in the registers
 *
 *  parameter | Register address |   bit
 *------------|------------------|----------------
 *	dig_T1    |  0x88 and 0x89   | from 0 : 7 to 8: 15
 *	dig_T2    |  0x8A and 0x8B   | from 0 : 7 to 8: 15
 *	dig_T3    |  0x8C and 0x8D   | from 0 : 7 to 8: 15
 *	dig_P1    |  0x8E and 0x8F   | from 0 : 7 to 8: 15
 *	dig_P2    |  0x90 and 0x91   | from 0 : 7 to 8: 15
 *	dig_P3    |  0x92 and 0x93   | from 0 : 7 to 8: 15
 *	dig_P4    |  0x94 and 0x95   | from 0 : 7 to 8: 15
 *	dig_P5    |  0x96 and 0x97   | from 0 : 7 to 8: 15
 *	dig_P6    |  0x98 and 0x99   | from 0 : 7 to 8: 15
 *	dig_P7    |  0x9A and 0x9B   | from 0 : 7 to 8: 15
 *	dig_P8    |  0x9C and 0x9D   | from 0 : 7 to 8: 15
 *	dig_P9    |  0x9E and 0x9F   | from 0 : 7 to 8: 15
 *	dig_H1    |         0xA1     | from 0 to 7
 *	dig_H2    |  0xE1 and 0xE2   | from 0 : 7 to 8: 15
 *	dig_H3    |         0xE3     | from 0 to 7
 *	dig_H4    |  0xE4 and 0xE5   | from 4 : 11 to 0: 3
 *	dig_H5    |  0xE5 and 0xE6   | from 0 : 3 to 4: 11
 *	dig_H6    |         0xE7     | from 0 to 7
 *
 *	@return results of bus communication function
 *	@retval 0 -> Success
 *	@retval -1 -> Error
 *
 *
*/

//
// Based Primarily on Embedded Adventures example source
// 

int BME280::logPartBSensorCal(std::ofstream & ofile)
{
  BME280::compParams_u cal_data;
	unsigned char txBuffer[2];		// transmit buffer
	uint8_t rdBuffer[12];		// transmit buffer  
  //-----------------

	txBuffer[0] = regCalibStart ;  // This is the address we want to read from.
 
	if ((opResult = write(i2cHandle, txBuffer, 1)) != 1) {
	  std::cout <<"BME280 : No ACK bit on Write in LogPartB" << std::endl;
    return -1;
  } 
  
  // The First 24 bytes of Cal data are contiguous.
  // The Union will handle filling dig_T* & dig_P*
  // 
  if((opResult = read(i2cHandle, cal_data.compArray, 24)) != 24) {
		std::cout <<"BME280 :: No ACK bit Read!" << std::endl;
    return -1;
	}
 
  //
  // H Calibration parameters
  //
 	txBuffer[0] = 0xA1 ;  // This is the address we want to read from. FIXME
  if ((opResult = write(i2cHandle, txBuffer, 1)) != 1) {
	  std::cout <<"BME280 : No ACK bit on Write in LogPartB" << std::endl;
    return -1;
  } 
  
  // *	dig_H1    |         0xA1     | from 0 to 7
  // 
  if((opResult = read(i2cHandle, rdBuffer, 1)) != 1) {
		std::cout <<"BME280 :: No ACK bit Read!" << std::endl;
    return -1;
	}
 
  // H1
  cal_data.compStruct.dig_H1 = rdBuffer[0];
  
  
 	txBuffer[0] = 0xE1 ;  // This is the address we want to read from. FIXME
  if ((opResult = write(i2cHandle, txBuffer, 1)) != 1) {
	  std::cout <<"BME280 : No ACK bit on Write in LogPartB" << std::endl;
    return -1;
  } 
 
 /*
 *	dig_H2    |  0xE1 and 0xE2   | from 0 : 7 to 8: 15
 *	dig_H3    |         0xE3     | from 0 to 7
 *	dig_H4    |  0xE4 and 0xE5   | from 4 : 11 to 0: 3
 *	dig_H5    |  0xE5 and 0xE6   | from 0 : 3 to 4: 11
 *	dig_H6    |         0xE7     | from 0 to 7
 */
  
  if((opResult = read(i2cHandle, rdBuffer, 7)) != 7) {
		std::cout <<"BME280 :: No ACK bit Read!" << std::endl;
    return -1;
	}
 
  cal_data.compStruct.dig_H2 = (rdBuffer[1] << 8 ) | rdBuffer[0];
  cal_data.compStruct.dig_H3 = rdBuffer[2];
  cal_data.compStruct.dig_H4 = (rdBuffer[3] << 4) | (rdBuffer[4] & 0xF);
  cal_data.compStruct.dig_H5 = (rdBuffer[5] << 4) | ((rdBuffer[3] >> 4) & 0xF);
  cal_data.compStruct.dig_H6 =  rdBuffer[6];
  
  //
  //
  // Write Cal data to ofile
  //
  ofile << "Sensor ID [" << std::hex << (int)uniqueID << "] Calibration Data ||" ;
  {
    // uint8_t *ptr = (void *) cal_data;
    for (unsigned int i = 0 ; i < sizeof(cal_data) ; i++)
    {
      ofile << std::hex << std::setfill('0') << std::setw(2) << (int)cal_data.compArray[i];
    }
  }
  ofile << "||" << std::endl ;
    
  // ofile << "||" << std::endl << "-----" << std::endl;
 
  return 0;
}
 
 
  //
// Eagle Aero Sport Extensions
//
EasDAQpack* BME280::fillEASpack(EasDAQpack &fillPack)
{
  fillPack.setID(EasDAQpack::BME280_t);
  fillPack.setUnique(uniqueID);
  
  fillPack.u.PresTempHum.Press = rawPress;
  fillPack.u.PresTempHum.Temp = rawTemp;
  fillPack.u.PresTempHum.Humid = rawHumid;
  
  return &fillPack;
}

