//
// EAS
//
// Brian Davis
//

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include <sys/ioctl.h>

#include <iostream>
#include <fstream>

#include "hscPress.h"
#include "eas_daq_pack.h"

//--------------------------------------------------------
// Constructor

HscPress::HscPress(int bus, int addr, uint8_t uniqueID_in) {
  #define MAX_BUS 40
  char namebuf[MAX_BUS];
  //-------------------
  blank();
  
  i2cBus = bus;
	snprintf(namebuf, sizeof(namebuf), "/dev/i2c-%d", i2cBus);

	//Create a file descriptor for the I2C bus
	if ((i2cHandle = open(namebuf, O_RDWR)) <= 0) {
		std::cout << "Failed to open HSC Pressure Sensor on " << namebuf << " I2C Bus" << std::endl;
    std::cout << "Error # = " << ((int)i2cHandle) << std::endl;
    return;
	}

  i2cAddress = addr;
	// Tell the I2C peripheral device address of the accelerometer
	if((opResult = ioctl(i2cHandle, I2C_SLAVE, i2cAddress)) < 0) {
		std::cout << "I2C_SLAVE address " << i2cAddress << " failed..." << std::endl;
		return;
	}
 
 uniqueID = uniqueID_in;
 
 init_device();
}

//---------------------------------------------
int HscPress :: init_device() {
  return 0;
}

//----------------------------------------------
//
// Destructor
//
HscPress::~HscPress() {
	// TODO Auto-generated destructor stub
  close(i2cHandle);
}

//---------------------------------------------
//
// Blank
void HscPress :: blank() {
    memset(this, 0, sizeof(HscPress));
}

//---------------------------------------------
//
// Update Data
int HscPress :: updatePressTemp() {
	unsigned char rxBuffer[6];		// receive buffer
  //--------------------------------------------------
  if((opResult = read(i2cHandle, rxBuffer, 4)) != 4) {
		std::cout <<"HscPress No ACK bit Read!" << std::endl;
    return -1;
	}

  status = (rxBuffer[0] & 0xc0) >> 6;  // first 2 bits from first byte
  bridge_data = ((rxBuffer[0] & 0x3f) << 8) + rxBuffer[1];
  temperature_data = ((rxBuffer[2] << 8) + (rxBuffer[3] & 0xe0)) >> 5;
  return 0;
}


//----------------------------------
//
// Eagle Aero Sport Extensions
//
EasDAQpack* HscPress::fillEASpack(EasDAQpack &fillPack)
{
  // fillPack.u_sensor_id = uniqueID;
  fillPack.setID(EasDAQpack::HSC_PRESS_t);
  fillPack.setUnique(uniqueID);

  // TODO
  // Fill Pack
  fillPack.u.hsc_press.status = status;
  fillPack.u.hsc_press.bridge_data = bridge_data;
  fillPack.u.hsc_press.temperature_data = temperature_data;
  
  return &fillPack;
}

//----------------------------------
//


int HscPress::logPartASensorID(std::ofstream & ofile, std::string sen_name)
{
  ofile << "Sensor unique ID : " << (int) uniqueID << std::endl;
  ofile << "Sensor type : Honeywell HSC/SSC pressure sensor " << std::endl;
  ofile << "Sensor bus : /dev/i2c-" << i2cBus << std::endl;
  ofile << "I2C Address : " << std::hex << i2cAddress << std::endl;
  ofile << "Sensor name / Placement : " << sen_name << std::endl;
  ofile << "-----" << std::endl;

  return 0;
}

//----------------------------------
//


uint8_t getStatus(){
  return status;
}

//----------------------------------
//


uint16_t getBridgeData(){
  return bridge_data;
}

//----------------------------------
//


uint16_t getTemperatureData(){
  return temperature_data;
}