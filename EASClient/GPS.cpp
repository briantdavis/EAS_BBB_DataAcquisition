#include "GPS.hpp"

using namespace CharacterBuffer;

GPS::GPS::GPS(std::string COMportLocation, int size) : CharacterBuffer::CharacterBuffer(size)
{
  /*
  O_RDRW:    Open Read/Write
  O_NOCTTY:  No control TTY
  O_NDELAY:  No delay
  */
  
  // ON BBB: COMportLocation is: "/dev/ttyO4"
  
  if((serialHandle = open(COMportLocation.c_str(),O_RDONLY | O_NOCTTY | O_NDELAY)) < 0 ){
    perror("UART: Failed to open the file.\n");
    //return ERROR_STOP;
  }
  struct termios options;
  tcgetattr(serialHandle, &options);
  /*
  B38400:  Baud rate of 38400
  CS8:     Size mask of 8 bits
  CREAD:   Enable receiver
  CLOCAL:  Ignore modem control lines (???????????)
  */
  options.c_cflag = B38400 | CS8 | CREAD | CLOCAL;
  /*
  IGNPAR:  Ignore framing and parity errors
  ICRNL:   Translate carriage return to newline on input (???????????)
  */
  options.c_iflag = IGNPAR | ICRNL;
  /*
  TCIFLUSH:  flushes data received but not read.

  */
  tcflush(serialHandle, TCIFLUSH);
  /*
  TCSANOW:  the change occurs immediately.

  */
  tcsetattr(serialHandle, TCSANOW, &options);
  //return SAFE;
}

int GPS::GPS::readGPS()
{
  char input;
  bool cont = true;
  int error = SAFE;
  
  while(cont){
    streamStatusCode = read(serialHandle, &input, 1);  // why did this compile w/o read?
    if(streamStatusCode < 0 && errno != EAGAIN){
      cont = false;
      sprintf(this->buffer, "read error = %d", streamStatusCode);
      perror(this->buffer);
      sprintf(this->buffer, "ERRNO = %d", errno);
      perror(this->buffer);
      error = ERR_READ;
    }
    else if(streamStatusCode == 0){
      cont = false;
      printf("there was no data available to read.");
      error = ERR_NOCHAR;
    }
    else if(input == 0){
      cont = false;
      printf("null char!\n");
      error = ERR_NOCHAR;
    }
    else if(input != '\n'){
      cont = false;
      int x = this->push(input);
      if(x == BUFFER_OVERFLOW_IMMINENT || x == BUFFER_OVERFLOW){
        //printf("overflowing!\n");
        error = ERROR_STOP;
      }
      else{
        //printf("ok.\n");
        error = SAFE;
      }
    }
    else{
      cont = false;
    }
  }
  return error;
}