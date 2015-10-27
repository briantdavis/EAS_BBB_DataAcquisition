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
#include <unistd.h>
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
#include "pca9544mux.h"
#include "mpu6050AccelGyro.h"

// -------------------------------------------
//
// #define
//
// Application / DAQ
//
#define ITTER_LOOP_PAUSE     1E6
#define MIN_ILOOPPERIOD      500

//
// I2C Space
//
#define BMP_180_ADDR	      0x77

#define I2C_HW1_BUS_NUM    2
#define I2C_HW2_BUS_NUM    1

//
// Types
// 

typedef struct shared_vars {
  bool gDone;
  bool gAccelEnabled;
  bool gPressEnabled;
  bool gTempEnabled;
  bool gLogFileEnabled;
  unsigned int iLoopPause;
} shared_vars_t;

//
// Globals
//

volatile static shared_vars_t *sv;

//
// Prototypes
//


//---------------------------------------------------
// #includes which require sv structure defn
//
#include "eas_tcon.h"


//---------------------------------------------------
//
// Main()
//
int main(int argc, char* argv[])
{
  PCA9544Mux i2cMuxNum1(I2C_HW2_BUS_NUM, PCA9544_000_ADDR);
  i2cMuxNum1.selectChan(PCA9544Mux :: CH_0);
  ADXL345Accelerometer accelNum1(I2C_HW2_BUS_NUM, ADXL345_SDO_H_ADDR);
  ADXL345Accelerometer accelNum2(I2C_HW2_BUS_NUM, ADXL345_SDO_L_ADDR);
  MPU6050AccelGyro accGyrNum3(I2C_HW2_BUS_NUM, MPU6050_AD0_L_ADDR);
  //i2cMuxNum1.selectChan(PCA9544Mux :: CH_1);
  //ADXL345Accelerometer accelNum3(I2C_HW1_BUS_NUM, ADXL345_SDO_H_ADDR);
  //ADXL345Accelerometer accelNum4(I2C_HW1_BUS_NUM, ADXL345_SDO_L_ADDR);
  i2cMuxNum1.selectChan(PCA9544Mux :: CH_2);
  ADXL345Accelerometer accelNum5(I2C_HW2_BUS_NUM, ADXL345_SDO_H_ADDR);
  ADXL345Accelerometer accelNum6(I2C_HW2_BUS_NUM, ADXL345_SDO_L_ADDR);
  //i2cMuxNum1.selectChan(PCA9544Mux :: CH_3);
  //ADXL345Accelerometer accelNum7(I2C_HW1_BUS_NUM, ADXL345_SDO_H_ADDR);
  //ADXL345Accelerometer accelNum8(I2C_HW1_BUS_NUM, ADXL345_SDO_L_ADDR);
  bmp180 baroSen(I2C_HW1_BUS_NUM);
  std::ofstream logFileStream;
    //----------------------------
  //
  // Set up shared memory space
  //
  sv = (shared_vars_t *) mmap(NULL, sizeof(shared_vars_t), PROT_READ | PROT_WRITE, 
                             MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  sv->gDone = false;
  sv->gAccelEnabled = true;
  sv->gPressEnabled = true;
  sv->gTempEnabled = true;
  sv->gLogFileEnabled = true;
  sv->iLoopPause = ITTER_LOOP_PAUSE;

  //
  // Setup TinyCon in Parallel Thread
  // 
  int tc_pid = fork();
  if (tc_pid == 0)
  {
    tcon tc(std::string(""));
    tc.run();
    std::cout << std::endl << "TinyCom thread ShutDown."<< std::endl;
    return 0;
  }
  
  //
  // Open Log File
  //
  if (sv->gLogFileEnabled)
  {
    #define LOG_NAME_SZ 120
    time_t now;
    char log_name[LOG_NAME_SZ] = {0};
    // -------------
    now = time(NULL);
    if (now == -1)
    {
      // ERROR HANDLING: you can check  to see what went wrong 
      perror("Failed to acquire date string for file in main");
      exit(1);
    }
    //-----
    strftime(log_name, LOG_NAME_SZ, "./logs/easRV12_%d_%b_%Y_%H_%M_%S.log", 
      gmtime(&now));
    logFileStream.open(log_name, std::ofstream::out | std::ofstream::app);
    if (!logFileStream.good())
    {
      // ERROR HANDLING: you can check errno to see what went wrong
      perror("Failed to open the logfile in main");
      std::cout << "Log File Name = " << log_name << std::endl;
      std::cout << "Errno = " << logFileStream << std::endl;
      exit(1);
    }
    std::cout << std::endl << "Log File " << log_name << " opened." << std::endl;
  } // if enabled

  //
  // Begin Repeated Loop
  //
  while (!sv->gDone)
  {
    EasDAQpack curPack;    
    //------------------------    
    //
    // Pressure & Temperature
    //
    baroSen.updateRawPressTemp();
    curPack.blank();
    baroSen.fillEASpack(curPack);
    if (logFileStream.good())
      curPack.outToFile(logFileStream);
    if (sv->gAccelEnabled)
      std::cout << "BaroTemp : " << curPack << std::endl;
    
    //
    // Select 00
    i2cMuxNum1.selectChan(PCA9544Mux :: CH_0);
    // std::cout << "Selecting Channel 0" << std::endl;
    
    //
    // Read from ADXL345 Num 1
    //
    accelNum1.updateAccelData();
    curPack.blank();
    accelNum1.fillEASpack(curPack);
    if (logFileStream.good())
      curPack.outToFile(logFileStream);
    if (sv->gAccelEnabled)
      std::cout << "Num 1 : " << curPack << std::endl; 
      
    
    //
    // Read from ADXL345 Num 2
    //
    accelNum2.updateAccelData();
    curPack.blank();
    accelNum2.fillEASpack(curPack);
    if (logFileStream.good())
      curPack.outToFile(logFileStream);
    if (sv->gAccelEnabled)
      std::cout << "Num 2 : " << curPack << std::endl;
      
    //
    // Read from MPU6050 Num 3
    //
    accGyrNum3.updateAccelGyroTempData();
    curPack.blank();
    accGyrNum3.fillEASpack(curPack);
    if (logFileStream.good())
      curPack.outToFile(logFileStream);
    if (sv->gAccelEnabled)
      std::cout << "Num 3 : " << curPack << std::endl;
      
      /*
    //
    // Select 01
    i2cMuxNum1.selectChan(PCA9544Mux :: CH_1);
    std::cout << "Selecting Channel 1" << std::endl;    
    //
    // Read from ADXL345 Num 3
    //
    accelNum3.updateAccelData();
    curPack.blank();
    accelNum3.fillEASpack(curPack);
    if (logFileStream.good())
      curPack.outToFile(logFileStream);
    if (sv->gAccelEnabled)
      std::cout << "Num 3 : " << curPack << std::endl; 
      
    
    //
    // Read from ADXL345 Num 4
    //
    accelNum4.updateAccelData();
    curPack.blank();
    accelNum4.fillEASpack(curPack);
    if (logFileStream.good())
      curPack.outToFile(logFileStream);
    if (sv->gAccelEnabled)
      std::cout << "Num 4 : " << curPack << std::endl;
      */
   
    //
    // Select 02
    i2cMuxNum1.selectChan(PCA9544Mux :: CH_2);
    // std::cout << "Selecting Channel 2" << std::endl;  
    
    //
    // Read from ADXL345 Num 5
    //
    accelNum5.updateAccelData();
    curPack.blank();
    accelNum5.fillEASpack(curPack);
    if (logFileStream.good())
      curPack.outToFile(logFileStream);
    if (sv->gAccelEnabled)
      std::cout << "Num 5 : " << curPack << std::endl; 
      
    
    //
    // Read from ADXL345 Num 6
    //
    accelNum6.updateAccelData();
    curPack.blank();
    accelNum6.fillEASpack(curPack);
    if (logFileStream.good())
      curPack.outToFile(logFileStream);
    if (sv->gAccelEnabled)
      std::cout << "Num 6 : " << curPack << std::endl;
      /*
      //
    // Select 03
    i2cMuxNum1.selectChan(PCA9544Mux :: CH_3);
        std::cout << "Selecting Channel 3" << std::endl;  
    
    //
    // Read from ADXL345 Num 5
    //
    accelNum7.updateAccelData();
    curPack.blank();
    accelNum7.fillEASpack(curPack);
    if (logFileStream.good())
      curPack.outToFile(logFileStream);
    if (sv->gAccelEnabled)
      std::cout << "Num 7 : " << curPack << std::endl; 
      
    
    //
    // Read from ADXL345 Num 6
    //
    accelNum8.updateAccelData();
    curPack.blank();
    accelNum8.fillEASpack(curPack);
    if (logFileStream.good())
      curPack.outToFile(logFileStream);
    if (sv->gAccelEnabled)
      std::cout << "Num 8 : " << curPack << std::endl;
      */
 
      
    // Pause 
    usleep(sv->iLoopPause);
    if (logFileStream.good())
    {
      clock_t t;
      t = clock();
      curPack.blank();
      curPack.setClockT(t);
      curPack.outToFile(logFileStream);
      std::cout << curPack << std::endl << std::endl;
    }
          
  } // while
  
  //
  // For clean shutdown, make sure child processes close first.
  //
  // http://stackoverflow.com/questions/5057137/c-loop-while-child-is-alive
  int Stat;
  // Blocking while()
  while (waitpid(tc_pid, &Stat, WNOHANG) != tc_pid);
  if (WIFEXITED(Stat)) {
    std::cout << "  parent: Child exited with exit code " << WEXITSTATUS(Stat) << std::endl;
  } else if (WIFSIGNALED(Stat)) {
    std::cout << "  parent: Child killed with signal " << WTERMSIG(Stat) << std::endl;
  } else {
    std::cout << "  parent: Something else happened to child, e.g. STOPPED" << std::endl;
  }
  
  //
  // Close Log File
  if (logFileStream.good())
  {
    std::cout << "  Closing Log File" << std::endl;
    logFileStream.close();
  }
  
  //
  // Free shared memory space
  munmap((void *)sv, sizeof(shared_vars_t));
  
  std::cout << "    Exiting Primary Thread Normally..." << std::endl;
  return 0;
} // main


