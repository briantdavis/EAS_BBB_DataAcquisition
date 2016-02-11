//
// EagleAeroSport
//
// B.Davis
//
//////////////////

#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>

#include "eas_daq_pack.h"

// EasDAQpack *test_packPtr;


EasDAQpack :: easDAQ_dispmode_t EasDAQpack :: active_dispmode = EasDAQpack :: SI;   

  //
  // Constructor
EasDAQpack :: EasDAQpack()
{
  //
  // Constructor
  blank();
}
//------------------------------------------
void EasDAQpack :: blank()
{
  memset(this, 0, sizeof(EasDAQpack));
  pack_id = UNDEF;
}
//------------------------------------
int EasDAQpack :: outToFile(std::ofstream &fptr)
{
  if ((!fptr.is_open()) || (!fptr.good()))
    return -1;

  fptr.write(reinterpret_cast <const char *> (this), sizeof(EasDAQpack));
  return 0; 
}   

//-------------------------------------------------
int EasDAQpack :: setID(easDAQpackId_t id)
{
  pack_id = id;
  return 0;
}
//-------------------------------------------------
int EasDAQpack :: setClockT(clock_t clk)
{
  pack_id = EasDAQpack :: CLOCK_T;
  u.clk_t = clk;
  return 0;
}
//-------------------------------------------------
int EasDAQpack :: setClockDual(clock_t crs, struct timespec pres)
{
  pack_id = EasDAQpack :: DUAL_CLOCK_t;
  u.time.course = crs;
  u.time.ns = pres;
  // @@@@ Working here TODO @@@@
  // u.clk_t = clk;
  return 0;
}

//---------------------------------------------------
int EasDAQpack :: outToConsole()
{
  std:: cout << this; 
  return 0;
}
//---------------------------------------------------
std::ostream &operator << (std::ostream &os,
			 const EasDAQpack &e)
{
#define ACCEL_SCALE (4)
switch (e.pack_id)
  {
  case EasDAQpack :: CLOCK_T:
    os << "Time = " << e.u.clk_t;
    break;
    
  case EasDAQpack :: DUAL_CLOCK_t:
    os << "Course Time = " << e.u.time.course << "  Precise Time = " <<
      e.u.time.ns.tv_sec << "." << e.u.time.ns.tv_nsec;
    break;
      
  case EasDAQpack :: ACCEL:
  case EasDAQpack :: ADXL345_t:
    if (EasDAQpack :: active_dispmode == EasDAQpack :: RAW)
    {
      os << "Accel ( " << std::dec << e.u.threeAxis_ss16.x << ", " <<
          e.u.threeAxis_ss16.y << ", " << e.u.threeAxis_ss16.z << ")";
    }
    else  if (EasDAQpack :: active_dispmode == EasDAQpack :: SI)
    {
      os << "Accel ( " << std::dec << std::fixed << std::setprecision(2) << ((float)e.u.threeAxis_ss16.x * 3.9 * 9.8) / 1000 << ", " <<
          ((float)e.u.threeAxis_ss16.y * 3.9 * 9.8) / 1000 << ", " << 
          ((float)e.u.threeAxis_ss16.z * 3.9 * 9.8) / 1000 << ")";
    }   
    break;
    
  case EasDAQpack :: BAROTEMP:
    os << "Baro = " << std::hex << e.u.barotemp.press << "\tTemp = " <<
          e.u.barotemp.temp;
    break;
    
  case EasDAQpack :: MPU6050_t:
    if (EasDAQpack :: active_dispmode == EasDAQpack :: RAW)
    {
      os << "Accel ( " << std::dec << e.u.accelGyro.accel_x << ", " <<
          e.u.accelGyro.accel_y << ", " << e.u.accelGyro.accel_z << ")";
          
      os << " Gyro ( " << std::dec << e.u.accelGyro.gyro_x << ", " <<
          e.u.accelGyro.gyro_y << ", " << e.u.accelGyro.gyro_z << ")";
          
      os << " Temp = " << std::dec << e.u.accelGyro.temp;
    }
    else if (EasDAQpack :: active_dispmode == EasDAQpack :: SI)
    {    
      os << "Accel ( " << std::dec << std::fixed << std::setprecision(2) << ((float)e.u.accelGyro.accel_x * ACCEL_SCALE * 9.8) / 0xFFFF << ", " <<
          ((float)e.u.accelGyro.accel_y * ACCEL_SCALE * 9.8) / 0xFFFF << ", " << 
          ((float)e.u.accelGyro.accel_z * ACCEL_SCALE * 9.8) / 0xFFFF << ")";
          
      os << " Gyro ( " << std::dec << e.u.accelGyro.gyro_x << ", " <<
          e.u.accelGyro.gyro_y << ", " << e.u.accelGyro.gyro_z << ")";
          
      os << " Temp = " << std::dec << e.u.accelGyro.temp;
    }
    break;

  case EasDAQpack :: BME280_t:
    os << "Baro = " << std::hex << e.u.PresTempHum.Press << "\tTemp = " <<
      e.u.PresTempHum.Temp << "\tHumid = " << e.u.PresTempHum.Humid;
    break;  

    case EasDAQpack :: HSC_PRESS_t:
      os << "HSC Pressure";
      switch (e.u.hsc_press.status)
      {
        case 0x3:
          os << " Fault";
          break;
        case 0x0:
          os << " = " << e.u.hsc_press.bridge_data << " Temp = " << e.u.hsc_press.temperature_data;
          break;
        case 0x1:
           os << " Cmd Mode";
          break;
        case 0x2:
           os << " Repeat Data";
          break;
      }
      break;               
    
  default:
    os << "Unknown Packet Ptype = " << e.pack_id;
  }
  return os;
}