/*
Header-File
bmp180.h
2015.06.18
*/

#include <cstdint>

#include "eas_daq_pack.h"

//
// Standard Guards
#ifndef bmp180_H_
#define bmp180_H_

#define BMP_180_ADDR	      0x77

//mode = 0 -> ulltralow
#define bmp_mode 0

//Adressen aus Datenblatt ausgelesen
#define bmp_ac1 0xAA
#define bmp_ac2 0xAC
#define bmp_ac3 0xAE
#define bmp_ac4 0xB0
#define bmp_ac5 0xB2
#define bmp_ac6 0xB4
#define bmp_b1 0xB6
#define bmp_b2 0xB8
#define bmp_mb 0xBA
#define bmp_mc 0xBC
#define bmp_md 0xBE

/*
Adressen um in register zu schreiben,lls

bzw zu lesen
*/
#define bmp_reg 0xF4
#define bmp_temperatur_raw 0x2E
#define bmp_pressure_raw 0x34
#define bmp_temp 0xF6
#define bmp_pres 0xF6

/*
This is Meaningless to Me
Die Klasse bmp180 soll dazu dienen die temperatur
und den druck zu lesen.
Die "unkompensierten"-Daten werden für die spätere berechnung
der Realwerte benötigt.
Temperatur wird in Grad=*C berechnet.
Druck wird in Pascale=Pa berechnet.
*/
    //
    // Typical values
    //
    // Example from Bosch datasheet
		// AC1 = 408; AC2 = -72; AC3 = -14383; AC4 = 32741; AC5 = 32757; AC6 = 23153;
		// B1 = 6190; B2 = 4; MB = -32768; MC = -8711; MD = 2868;

		// Example from http://wmrx00.sourceforge.net/Arduino/BMP180-Calcs.pdf
		// AC1 = 7911; AC2 = -934; AC3 = -14306; AC4 = 31567; AC5 = 25671; AC6 = 18974;
		// VB1 = 5498; VB2 = 46; MB = -32768; MC = -11075; MD = 2432;

class bmp180
{
  private:  
    int16_t ac1,ac2,ac3,b1,b2,mb,mc,md;
    uint16_t ac4,ac5,ac6;
    uint8_t oss;
    
    uint16_t rawTemp;
    uint16_t rawPress;
    
    int i2cBusPtr;
    
    uint16_t updateRawTemp();
    uint32_t updateRawPress();
  
  public:
    bmp180(int);
    // From Buffered Values
    int32_t readPressure();
    float readTemp();
    EasDAQpack* fillEASpack(EasDAQpack &);
    
    // From Device
    int readCalValues();
    uint32_t updateRawPressTemp();
};
  
#endif
