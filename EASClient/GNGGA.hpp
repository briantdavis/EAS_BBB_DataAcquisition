#ifndef GNGGA_HPP
#define GNGGA_HPP

#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "functionLibrary.hpp"

namespace GNGGA
{
  class GNGGA
  {
    private:
      //hdop: Horizontal dilution of precision 00.0-99.9
      float  seconds, latitude, longitude, hdop, altitude, geoidHeight;
      //GPSQuality: 0 (no fix), 1 (valid, SPS), 2 (valid, differential GPS)
      //dgpsID: 0000-1024 or NULL (ID of differential reference station or Null)
      unsigned int hours, minutes;
      //N:  if N is true, then north, otherwise south
      //E:  if E is true, then east, otherwise west
      bool N, E;
    public:
      GNGGA(std::string);
      float getTime();
      float getLat();
      float getLong();
      float getAlt();
      void print();
  };

  bool isGNGGA(std::string);
}

#endif