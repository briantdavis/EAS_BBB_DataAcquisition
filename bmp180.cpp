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

#include "bmp180_bosch.h"
#include "bmp180.h"

#ifndef BMP180_EAS_H_
#define BMP180_EAS_H_


#define BMP_180_ADDR	      0x77

//
//Default Constructor
// Example from Bosch datasheet
		// AC1 = 408; AC2 = -72; AC3 = -14383; AC4 = 32741; AC5 = 32757; AC6 = 23153;
		// B1 = 6190; B2 = 4; MB = -32768; MC = -8711; MD = 2868;

		// Example from http://wmrx00.sourceforge.net/Arduino/BMP180-Calcs.pdf
		// AC1 = 7911; AC2 = -934; AC3 = -14306; AC4 = 31567; AC5 = 25671; AC6 = 18974;
		// VB1 = 5498; VB2 = 46; MB = -32768; MC = -11075; MD = 2432;
bmp180::bmp180(int bus){
    
  ac1 = 408;
  ac2 = -72;
  ac3 = -14383;
  ac4 = 32741;
  ac5 = 32757;
  ac6 = 23153;
   
  b1 = 6190;
  b2 = 4;
  mb = -32768; 
  mc = -8711; 
  mc = 2868;
  
  readCalValues();

  oss = bmp_mode;
  
  rawTemp = 0;
  rawPress = 0;
  
  //
  // Open I2C Bus
  //
  #define MAX_BUS 24
  char i2cnamebuf[MAX_BUS];
	snprintf(i2cnamebuf, sizeof(i2cnamebuf), "/dev/i2c-%d", bus);
  if ((i2cBusPtr = open(i2cnamebuf, O_RDWR)) < 0) {
      /* ERROR HANDLING: you can check errno to see what went wrong */
      perror("BMP180 : Failed to open the i2c bus");
      return;
  }
  
  //
  // Associate BMP I2C Addr w/ Bus Ptr
  //
  if (ioctl(i2cBusPtr, I2C_SLAVE, BMP_180_ADDR) < 0) {
      printf("BMP180 : Failed to acquire bus access and/or talk to slave.\n");
      /* ERROR HANDLING; you can check errno to see what went wrong */
      return;
  }
}

/*
mode wird festgelegt auf 0
Alle kallibrierungswerte werden ausgelesen
*/
//bool bmp180::begin(){

   // @@@ Add a Read of the Calibration Values
  // readCalValues();  

  //Wird für weitere operationen benötigt  
  // Wire.begin();
  
  //kallibrierungs Werte auslesen
  //ac1 = read16(bmp_ac1);
  //ac2 = read16(bmp_ac2);
  //ac3 = read16(bmp_ac3);
  //ac4 = read16(bmp_ac4);
  //ac5 = read16(bmp_ac5);
  //ac6 = read16(bmp_ac6);
  //b1 = read16(bmp_b1);
  //b2 = read16(bmp_b2);
  //mb = read16(bmp_mb);
  ///mc = read16(bmp_mc);
  //md = read16(bmp_md);
 
//return true;
//}

int bmp180::readCalValues(){
return 0;
}
  

/*
Temperatur aus den ausgelesenen Werten berechnen
Die Temperatur muss als float wert zurückgegeben werden
damit eine gleitkommazahl ausgegeben werden kann
*/
float bmp180::readTemp(){
  int32_t ut,b5;
  float temperatur;
  
  //ut = uncompensated temperature
  ut = rawTemp;
  //();
  
  /*
  Berechnug der realen Temperatur in *C
  Berechnungen aus dem Datenblatt ausgelesen
  */
  int32_t x1 = ((ut - (int32_t)ac6) * (int32_t)ac5) >> 15;
  int32_t x2 = ((int32_t)mc << 11) / (x1 + (int32_t)md);
  
  b5 = x1 + x2;

  temperatur = (b5 +8) >> 4;
  temperatur /= 10.0;
  
  return temperatur;
}

/*
Druck aus den ausgelesenen Werten berechnen
Der Druck kann als int32_t zurück gegeben werden
da er un Pascale angezeigt wird.
(Bei umrechnung in bar muss über einen neunen
Rückgabetyp nachgedacht werden!)
*/
int32_t bmp180::readPressure(){
 int32_t ut,up,b3,b5,b6,x1,x2,x3,p;
 uint32_t b4,b7;
 
  //ut = uncompensated temperature
  //up = uncompensated pressure
  //up = readRawPres();
  //ut = readRawTemp();
  up = rawPress;
  ut = rawTemp;
  
  /*
  Berechnung des realen Drucks in Pascale
  Berechnungen aus dem Datenblatt ausgelesen
  */
  int32_t x11 = ((ut - (int32_t)ac6) * (int32_t)ac5) >> 15;
  int32_t x22 = ((int32_t)mc << 11) / (x11 + (int32_t)md);
  
  b5 = x11 + x22;
  
  b6 = b5 - 4000;
  x1 = ((int32_t)b2 * ((b6 * b6) >> 12)) >> 11;
  x2 = ((int32_t)ac2 * b6) >> 11;
  x3 = x1 + x2;
  b3 = (((((int32_t)ac1*4) + x3) << oss) + 2) / 4;
  x1 = ((int32_t)ac3 * b6) >> 13;
  x2 = (((int32_t)b1 * (b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  b4 = ((int32_t)ac4 * (uint32_t)(x3 + 32768)) >> 15;
  b7 = ((uint32_t)up - b3) * (uint32_t)(50000 >> oss);
  if (b7 < 0x80000000){
    p = (b7 * 2) / b4;
  }
  else{
    p = (b7 / b4) * 2;
  }
  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  p = p + ((x1 + x2 + (int32_t)3791) >> 4);
  
  return p;
}

//-------------------------------------------------------------------
// readRawTemp ( )
//
// Primary function - read temperature data from BMP180 device
//
uint16_t bmp180::updateRawTemp(){
  char buf[20] = {0};

  //
  // Start Temperature
  //
  buf[0] = BMP180_CTRL_MEAS_REG;;
  buf[1] = BMP180_T_MEASURE;
  write(i2cBusPtr, buf, 2);
  
  // pause
  // Temperature Conversion 4.5 mS
  // Page 21 of datasheet Rev 2.5
  usleep(5E3);
  
  //
  // Begin Read from ADC MSB
  //
  buf[0] = BMP180_ADC_OUT_MSB_REG;
  write(i2cBusPtr, buf, 1);
  
   // Using I2C Read
    if (read(i2cBusPtr,buf,2) != 2) {
      /* ERROR HANDLING: i2c transaction failed */
      std::cout << "bmp180_rrT Failed to read from i2c bus: " <<  strerror(errno) << std::endl << std::endl;
      return 0;
    } else {
      // Correct Read of (2) bytes
      rawTemp = (buf[0] << 8) | buf[1];
      //
      // Display Temp Value
        // std::cout << "raw Temp value     = 0x"<< std::hex << rawTemp << std::endl;       
    }
  return rawTemp;
}

//-------------------------------------------------------------------
// readRawPress ( )
//
// Primary function - read pressure data from BMP180 device
//  
uint32_t bmp180::updateRawPress(){
    char buf[20] = {0};
  //
  // Start Pressure
  //
  buf[0] = BMP180_CTRL_MEAS_REG;
  buf[1] = BMP180_P_MEASURE;
  write(i2cBusPtr, buf, 2);
  
  // pause
  // Pressure Conversion 4.5 - 25.5 mS
  // Page 21 of datasheet Rev 2.5
  usleep(26E3);
  
  //
  // Begin Read from ADC MSB
  //
  buf[0] = BMP180_ADC_OUT_MSB_REG;
  write(i2cBusPtr, buf, 1);
  
   // Using I2C Read
    if (read(i2cBusPtr,buf,2) != 2) {
        /* ERROR HANDLING: i2c transaction failed */
        std::cout << "bmp180_rrP Failed to read from i2c bus: " << strerror(errno) << std::endl;
        return 0;
    } else {
      rawPress = (buf[0] << 8) | buf[1];
      //
      // Display Press Value    
      //std::cout << "raw Pressure value = 0x" << std::hex << rawPress << std::endl;
    }
return rawPress;        
}

//------------------------------------------------------------
//
//
uint32_t bmp180::updateRawPressTemp(){

  //
  // Initiating Communication with BMP180
  //
  /*
  if (ioctl(i2cBusPtr, I2C_SLAVE, BMP_180_ADDR) < 0) {
      printf("Failed to acquire bus access and/or talk to slave.\n");
      // ERROR HANDLING; you can check errno to see what went wrong 
      exit(1);
  }
  */
  
  updateRawTemp();
  
  updateRawPress();
  
  return rawPress;
}
//------------------------------------------------------------
//
//
EasDAQpack* bmp180::fillEASpack(EasDAQpack &fillPack)
{
  fillPack.setID(EasDAQpack::BAROTEMP);
  fillPack.u.barotemp.temp = rawTemp;
  fillPack.u.barotemp.press = rawPress;
  // fillPack.u.threeAxis_ss16.z = rawData_Z;
  return &fillPack;
}





#endif