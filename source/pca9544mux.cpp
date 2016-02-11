// -----------------------
// PCA 9544
// C++ Library
//
// Eagle AeroSport
// B.Davis
//

// -----------------------
//
//

// -----------------------
// Includes
//

#include <fcntl.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include <iostream>
#include <cstring>

#include "pca9544mux.h"

// -----------------------
//
//

PCA9544Mux :: PCA9544Mux(int bus, int addr)
{
#define MAX_BUS 40
  char namebuf[MAX_BUS];
  //-------------------
  blank();
  i2cBus = bus;
	snprintf(namebuf, sizeof(namebuf), "/dev/i2c-%d", i2cBus);

	//Create a file descriptor for the I2C bus
	if ((i2cHandle = open(namebuf, O_RDWR)) <= 0) {
		std::cout << "Failed to open PCA9544a Mux on " << namebuf << " I2C Bus" << std::endl;
    std::cout << "Error # = " << ((int)i2cHandle) << std::endl;
    return; //  (1);
	}
 
  i2cAddress = addr;
	// Tell the I2C peripheral device address of the accelerometer
	if((opResult = ioctl(i2cHandle, I2C_SLAVE, i2cAddress)) < 0) {
		std::cout << "I2C_SLAVE address " << i2cAddress << " failed..." << std::endl;
		return; // (2);
	}
 
  selectChan(CH_0);
}

// -----------------------
//
//
void PCA9544Mux :: blank() {
  memset(this, 0, sizeof(PCA9544Mux));
  chanSelected = PCA9544Mux :: UNDEF;
}
// -----------------------
//
//
int PCA9544Mux :: selectChan(muxChan new_chan)
{
  unsigned char txBuf[2];
  
  //
  // Select I2C address for this device
  //

  //
  // Page 12 of datasheet
  // http://www.ti.com/product/pca9544a?qgpn=pca9544a
  txBuf[0] = 0x4 | (((unsigned int)new_chan) & 0x3);
  
  // std::cout << "Mux Ctrl Word = 0x" << std::hex << ((int)txBuf[0]) << std::endl;
   
  if ((opResult = write(i2cHandle, txBuf, 1)) != 1) {
	  std::cout <<"PCA9544 No ACK bit Write!" << std::endl;
	}
   chanSelected = new_chan;
  // std::cout << "Mux_Chan = " << chanSelected << std::endl;
  return 0;
}

// -----------------------
//
//
//
// Destructor
//
PCA9544Mux::~PCA9544Mux() {
	// TODO Auto-generated destructor stub
}
