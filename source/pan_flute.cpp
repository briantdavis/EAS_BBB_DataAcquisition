//
// EAS
//
// Brian Davis
//
// I2C development Code

//
// ANSI C includes
//
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>         // Shared Memory mmap/munmap
#include <fcntl.h>
#include <sys/wait.h>         // waitpid

//
// C++ includes
//
#include <iostream>
#include <istream>
#include <fstream>

//
// Project specific includes
//
#include "libs/tc/tinycon.h"

#include "eas_daq_pack.h"

#include "bmp180_bosch.h"
#include "ADXL345Accelerometer.h"
#include "bmp180.h"
#include "bme280.h"
#include "pca9544mux.h"
#include "mpu6050AccelGyro.h"
#include "hscPress.h" 

#include <cmath>

//
// I2C Space
//

#define I2C_HW1_BUS_NUM    2
#define I2C_HW2_BUS_NUM    1

// Notes to play. {ab2 = 103.826, c3 = 130.813, eb3 = 155.563, g3 = 195.998, c4 = 261.626, eb4 = 311.127}.
int main(){
	
	using namespace std;
  
  uint8_t timeStamp_UID;
  //----------------------------
  // ***
  // Set up shared memory space
  //
  sv = (shared_vars_t *) mmap(NULL, sizeof(shared_vars_t), PROT_READ | PROT_WRITE, 
                             MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  sv->gDone = false;
  sv->gDispEnabled = true;
  sv->gLogFileEnabled = true;
  sv->iLoopPause = ITTER_LOOP_PAUSE;
  sv->gNextUniqueID = 0;
  timeStamp_UID = (++sv->gNextUniqueID);
	
  uint16_t basePress[6] = {0};
  int addr[6] = {0x28, 0x29, 0x33, 0x34, 0x35, 0x36};          //<<<<<The I2C address of the slave
  float notes[6] = {103.862, 130.813, 155.563, 195.998, 261.626, 311.127};
  
  HscPress press[6] = {HscPress(I2C_HW2_BUS_NUM, addr[0], ++sv->gNextUniqueID),
                       HscPress(I2C_HW2_BUS_NUM, addr[1], ++sv->gNextUniqueID),
                       HscPress(I2C_HW2_BUS_NUM, addr[2], ++sv->gNextUniqueID),
                       HscPress(I2C_HW2_BUS_NUM, addr[3], ++sv->gNextUniqueID),
                       HscPress(I2C_HW2_BUS_NUM, addr[4], ++sv->gNextUniqueID),
                       HscPress(I2C_HW2_BUS_NUM, addr[5], ++sv->gNextUniqueID)}; 

  //initialize the base pressures for later comparison.
  for(int i = 0; i < 6; i++)
  {
    press[i].updatePressTemp();
    basePress[i] = press[i].getBridgeData();
  }
  
  while(true)
  {
    int sensorMax = 0;
    int sensorNum = -1;

    for(int i = 0; i < 6; i++)
    {
      press[i].updatePressTemp();
      int currPress = press[i].getBridgeData;
      int currBasePress = basePress[i];
      if(currPress > (currBasePress + 50))
      {
        int pressDiff = currPress - currBasePress;
        if(pressDiff > sensorMax){
          sensorMax = pressDiff;
          sensorNum = i;
        }
      }
    }

    

  }
}
