#ifndef GPS_HPP
#define GPS_HPP

#include <string>
#include <termios.h>
#include <errno.h>
#include <fstream>
#include <fcntl.h>

#include "CharacterBuffer.hpp"

namespace GPS
{
  static int const SAFE = 0,
                 ERR_NOCHAR = 1,
                 ERR_READ = -1,
                 ERROR_STOP = -2;

  class GPS : public CharacterBuffer::CharacterBuffer
  {
    private:
      int serialHandle,
          streamStatusCode;
    public:
      GPS(std::string, int);
      int readGPS();
  };
}

#endif // GPS_HPP