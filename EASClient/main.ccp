#include "GNGGA.hpp"
#include "GPS.hpp"
#include "functionLibrary.hpp"

#include <iostream>

int main()
{
  GPS::GPS GeoPosSys("/dev/ttyO4",1024);
  std::vector<GNGGA::GNGGA> GNGGAstrings;
  
  bool runflag = true;
  std::string CurrentNMEA = "";
  do
  {
    //std::cout << "I'm reading the buffer" << std::endl;
    while(GeoPosSys.readGPS() != GPS::ERROR_STOP); //Fill the GPS Buffer
    //std::cout << "I've read the buffer" << std::endl;
    GeoPosSys.readBetween(CurrentNMEA,'$'); //Get the next NMEA String
    if(GNGGA::isGNGGA(CurrentNMEA)) //check to see if the gotten NMEA is a GNGGA sentence
    {
      GNGGA::GNGGA newGNGGA(CurrentNMEA);
      newGNGGA.print();
      //std::cout << "I'm here" << std::endl;
      GNGGAstrings.push_back(newGNGGA);
    }
    else
    {
      //NMEA Strings are returning empty
      //std::cout << CurrentNMEA << std::endl;
      //std::cout << GeoPosSys.str() << std::endl;
      //std::cout << GeoPosSys.loc() << std::endl;
    }
  } while(runflag);
}