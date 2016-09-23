//
// EAS
//
// Brian Davis
//

#include "mpu6050AccelGyro.h"
#include "eas_daq_pack.h"

MPU6050AccelGyro :: MPU6050AccelGyro(int bus, int addr) {
  #define MAX_BUS 40
  char namebuf[MAX_BUS];
  //-------------------
  blank();
  i2cBus = bus;
	snprintf(namebuf, sizeof(namebuf), "/dev/i2c-%d", i2cBus);

	//Create a file descriptor for the I2C bus
	if ((i2cHandle = open(namebuf, O_RDWR)) <= 0) {
		std::cout << "Failed to open MPU6050 on " << namebuf << " I2C Bus" << std::endl;
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
}

void MPU6050AccelGyro :: blank() {
    memset(this, 0, sizeof(MPU6050AccelGyro));
}

int MPU6050AccelGyro :: init_device() {
  unsigned char rxBuffer[16];		// receive buffer
	unsigned char txBuffer[4];		// transmit buffer
  unsigned char reg_val;

  //
  // Set Sleep Bit to ZERO
  //
  // Set Clock Source to X Gyro
  //
  //    int8_t power = i2c_smbus_read_byte_data(fd, MPU_POWER1); == 0x6b
  //  i2c_smbus_write_byte_data(fd, MPU_POWER1, ~(1 << 6) & power);
  //
  // Read
  txBuffer[0] = MPU6050_RA_PWR_MGMT_1;
  if ((opResult = write(i2cHandle, txBuffer, 1)) != 1) {
	  std::cout <<"MPU6050 No ACK bit Write!" << std::endl;
    return -1;
  }
  //
  if((opResult = read(i2cHandle, rxBuffer, 1)) != 1) {
		std::cout <<"MPU6050 No ACK bit Read!" << std::endl;
    return -1;
	}
  // Modify
  reg_val = rxBuffer[0] & (~(1<<6)) & (~0x7) ; // clear bits
  reg_val = reg_val | (0x1) ; // set bits
  txBuffer[1] = reg_val;
  
  // Write
  if ((opResult = write(i2cHandle, txBuffer, 2)) != 2) {
	  std::cout <<"MPU6050 No ACK bit Write!" << std::endl;
    return -1;
  }
  
  //
  // Set 
    
  return 0;
}


int MPU6050AccelGyro :: updateAccelGyroTempData() {
	unsigned char rxBuffer[16];		// receive buffer
	unsigned char txBuffer[1];		// transmit buffer

	txBuffer[0] = MPU6050_RA_ACCEL_XOUT_H ;  // This is the address we want to read from.
 
	if ((opResult = write(i2cHandle, txBuffer, 1)) != 1) {
	  std::cout <<"MPU6050 No ACK bit Write!" << std::endl;
    return -1;
  } 
  
  if((opResult = read(i2cHandle, rxBuffer, 14)) != 14) {
		std::cout <<"MPU6050 No ACK bit Read!" << std::endl;
    return -1;
	}

	rawAccel_X_data = (rxBuffer[0]<<8) | rxBuffer[1];
	rawAccel_Y_data = (rxBuffer[2]<<8) | rxBuffer[3];
	rawAccel_Z_data = (rxBuffer[4]<<8) | rxBuffer[5];
  rawTemp_data    = (rxBuffer[6]<<8) | rxBuffer[7];
  rawGyro_X_data  = (rxBuffer[8]<<8) | rxBuffer[9];
	rawGyro_Y_data  = (rxBuffer[10]<<8) | rxBuffer[11];
	rawGyro_Z_data  = (rxBuffer[12]<<8) | rxBuffer[13]; 

	return (0);
 }
 
 
 //
// Eagle Aero Sport Extensions
//
EasDAQpack* MPU6050AccelGyro::fillEASpack(EasDAQpack &fillPack)
{
  fillPack.setID(EasDAQpack::MPU6050_t);
  fillPack.u.accelGyro.accel_x = rawAccel_X_data;
  fillPack.u.accelGyro.accel_y = rawAccel_Y_data;
  fillPack.u.accelGyro.accel_z = rawAccel_Z_data;
  
  fillPack.u.accelGyro.gyro_x = rawGyro_X_data;
  fillPack.u.accelGyro.gyro_y = rawGyro_Y_data;
  fillPack.u.accelGyro.gyro_z = rawGyro_Z_data;
  
  fillPack.u.accelGyro.temp = rawTemp_data;
  
  return &fillPack;
}

//
// Destructor
//
MPU6050AccelGyro::~MPU6050AccelGyro() {
	// TODO Auto-generated destructor stub
}

