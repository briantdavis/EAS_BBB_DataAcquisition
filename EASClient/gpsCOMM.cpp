#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <fstream>

class CharacterBuffer{
  public:
    /*
    @static-integer-constant:
      ERROR_RIDINGBUFFER:  A general error code that warns against an immenent collision of buffer positions.
      ERROR_COLLISION:     A general error code that warns that retrieving or placing data was not possible.
      SAFE:                The general status code for a method that has encountered no errors.
      
      RIDING_OUTPUT:       A buffer status code that defines the state during which the input position is approaching a collision with the output position.
      RIDING_NOTHING:      A buffer status code that defines the state during which no type of collision is imminent.
      RIDING_INPUT:        A buffer status code that defines the state during which the output position is approaching a collision with the input position.
    */
    static int const ERROR_RIDINGBUFFER = -2,
                     ERROR_COLLISION = -1,
                     SAFE = 0,
                     RIDING_OUTPUT=1,
                     RIDING_NOTHING=0,
                     RIDING_INPUT=-1;
    /*
    @integer:
      bufferSize:       The number of elements within the buffer.
      bufferInputPosition:   The position that new data will be pushed to next.
      bufferOutputPosition:  The position that data will be pulled from next.
     statusCode:            The last status code as defined by the last function ran.
      bufferRidingStatus:    The current status of the buffer riding position.
    */
    int  bufferSize,
         bufferInputPosition,
         bufferOutputPosition,
         statusCode,
         bufferRidingStatus;
    /*
    @charachter:
      *buffer:  The data structure that contains all of the valuable information.
    */
    char *buffer;
    
    
    /*
    function:  defaultBuffer
      @param:  bufferSize
        The desired size of the buffer.
        
      This function initializes the buffer with the given size 'bufferSize'.
      The buffer is oriented in a 'RIDING_INPUT' configuration at the 0th position.
      'lastError' is set to the default 'SAFE' condition.
    */
    void defaultBuffer(int sizeDefinition){
      bufferInputPosition = 0;
      bufferOutputPosition = 0;
      buffer = new char[bufferSize = sizeDefinition];
      statusCode = SAFE;
      bufferRidingStatus = RIDING_INPUT;
    }
    
    /*
    function:  push
      @return-type: Integer
        The returned integer is the buffer status code.
      @param:  input
        Type:  charachter
        The character to be placed at the current 'bufferInputPosition'
        
      The function first checks if 'bufferInputPositon' is riding 'bufferOutputPosition'.
      If the aforementioned is false, then 'input' is placed into the 'inputBuffer' and 'inputBufferPosition' is simultaneously incremented within the allowable buffer positions as defined by 'inputBufferSize'.
      Otherwise, simultaneously return and set 'lastError' as 'ERROR_COLLISION'.
       Within the aforementioned if statementand and after its defining statements, check if the 'bufferInputPosition' is now equal to the 'bufferOutputPositon'. Should the statement be true, define 'bufferRidingStatus' as 'RIDING_OUTPUT' and simultaneously return and set 'statusCode' as 'ERROR_RIDINGBUFFER'.
       Otherwise, define 'bufferRidingStatus' as 'RIDING_NOTHING' and simultaneously return and set 'statusCode' as 'SAFE'.
    */
    int push(char input){
      if(bufferRidingStatus != RIDING_OUTPUT){
        buffer[bufferInputPosition++] = input;
        bufferInputPosition = bufferInputPosition % bufferSize;
        if(bufferInputPosition == bufferOutputPosition){
          bufferRidingStatus = RIDING_OUTPUT;
          return statusCode = ERROR_RIDINGBUFFER;
        }
        else{
          bufferRidingStatus = RIDING_NOTHING;
          return statusCode = SAFE;
        }
      }
      else{
        return statusCode = ERROR_COLLISION;
      }
    }
    /*
    function:  pull
      @return-type: Integer
        The returned integer is the buffer status code.
      @param:  *output
        Type:  charachter
        The memory pointer to a character that will accept the output of this function 
        
      The function first checks if 'bufferOuputPositon' is riding 'bufferInputPosition'.
      If the aforementioned is false, then '*output' is defined as the data in 'inputBuffer' at 'outputBufferPosition'. 'outputBufferPosition' is simultaneously incremented during the aforementioned command within the allowable buffer positions as defined by 'inputBufferSize'.
      Otherwise, simultaneously return and set 'statusCode' as 'ERROR_COLLISION'.
       Within the aforementioned if statementand and after its defining statements, check if the 'bufferInputPosition' is now equal to the 'bufferOutputPositon'. Should the statement be true, define 'bufferRidingStatus' as 'RIDING_INPUT' and simultaneously return and set 'statusCode' as 'ERROR_RIDINGBUFFER'.
       Otherwise, define 'bufferRidingStatus' as 'RIDING_NOTHING', define '*output' as 'null', and simultaneously return and set 'statusCode' as 'SAFE'.
    */
    int pull(char *output){
      if(bufferRidingStatus != RIDING_INPUT){
        *output = buffer[bufferOutputPosition++];
        bufferOutputPosition = bufferOutputPosition % bufferSize;
        if(bufferInputPosition == bufferOutputPosition){
          bufferRidingStatus = RIDING_INPUT;
          return statusCode = ERROR_RIDINGBUFFER;
        }
        else{
          bufferRidingStatus = RIDING_NOTHING;
          return statusCode = SAFE;
        }
      }
      else{
        return statusCode = ERROR_COLLISION;
      }
    }
    
    int poll(char *output){
      *output = buffer[bufferOutputPosition];
      return statusCode;
    }
} gpsBuffer;

int serialHandle,
    streamStatusCode;

static int const SAFE = 0,
                 ERR_NOCHAR = 1,
                 ERR_READ = -1,
                 ERR_SHTAP = -2;

int setup(){
  /*
  O_RDRW:    Open Read/Write
  O_NOCTTY:  No control TTY
  O_NDELAY:  No delay
  */
  if((serialHandle = open("/dev/ttyO4",O_RDONLY | O_NOCTTY | O_NDELAY)) < 0 ){
    perror("UART: Failed to open the file.\n");
    return -2;
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
  return SAFE;
}



int readGPS(){
  char input;
  bool cont = true;
  int error = SAFE;
  
  while(cont){
    streamStatusCode = read(serialHandle, &input, 1);
    if(streamStatusCode < 0 && errno != EAGAIN){
      cont = false;
      sprintf(gpsBuffer.buffer, "read error = %d", streamStatusCode);
      perror(gpsBuffer.buffer);
      sprintf(gpsBuffer.buffer, "ERRNO = %d", errno);
      perror(gpsBuffer.buffer);
      error = ERR_READ;
    }
    else if(streamStatusCode == 0){
      cont = false;
      printf("there was no data available to read.");
      error = ERR_NOCHAR;
    }
    else if(input == 0){
      cont = false;
      //("null char!\n");
      error = ERR_NOCHAR;
    }
    else if(input != '\n'){
      cont = false;
      int x = gpsBuffer.push(input);
      if(x == gpsBuffer.ERROR_RIDINGBUFFER){
        error = ERR_SHTAP;
      }
      else{
        error = SAFE;
      }
    }
    else{
      cont = false;
    }
  }
  return error;
}



int getNMEAstr(std::string *output){
  char input, newNMEA = '$', future;
  bool read = false, continuity = true;
  printf("grabbing NMEA\n");
  //while((gpsBuffer.pull(&input) != gpsBuffer.ERROR_COLLISION) && continuity){
  while(continuity && (gpsBuffer.pull(&input) != gpsBuffer.ERROR_COLLISION)){
    gpsBuffer.poll(&future);
    if(input == newNMEA && !read){
      read = true;
    }
    else if(future == newNMEA && read){
      continuity = false;
    }
    
    if(read){
      *output += input;
    }
  }
  return SAFE;
}

int poll(){
  bool continuity = true;
  while(continuity){
    try{
      std::string tempNMEA = "";
      int error = getNMEAstr(&tempNMEA);
      if(error != SAFE){
        printf("ERROR RETURNED: %d\n",error);
      }
      
      
      if(tempNMEA != ""){
        printf("NOT EMPTY! %s\n",tempNMEA.c_str());
        return SAFE;
      }
      else{
        printf("empty string returned\n");
        continuity = false;
      }
    }//END TRY
    catch(std::exception e){
      perror(e.what());
      printf("WARNING!!");
    }
  }
  return SAFE;
}
/*
struct nmea_data{
	std::string
}

int parse(std::vector<>){
	
}*/

int main(){
  /*struct CharachterBuffer test;
  test.defaultBuffer(7);
  char pull;
  printf("empty pull (output riding input) test result: %d\n",test.pull(&pull));
  printf("empty push test result: %d\n",test.push('$'));
  printf("immenent output riding input occupied pull test result: %d\n",test.pull(&pull));
  printf("data integrity check: %c\n",pull);
  printf("output riding input push test result: %d\n",test.push('H'));
  printf("input && output riding nothing push test result: %d\n",test.push('I'));
  printf("input && output riding nothing push test result: %d\n",test.push(' '));
  printf("input && output riding nothing push test result: %d\n",test.push('M'));
  printf("input && output riding nothing push test result: %d\n",test.push('O'));
  printf("input && output riding nothing push test result: %d\n",test.push('M'));
  printf("immenent input riding output push test result: %d\n",test.push('!'));
  printf("input riding output push test result: %d\n",test.push('?'));
  
  
  int flow = 0;
  std::string gather = "";
  while((flow = test.pull(&pull)) != test.ERROR_COLLISION){
    printf("The input int is: %d\n",flow);
    gather += pull;
    if(flow == test.ERROR_COLLISION){
      printf("WHOA, DUDE!");
      break;
    }
  }
  printf("%s\n\n\n\n**********\n\n\n",gather.c_str());
  printf("bufferOutput: %d\n",test.bufferOutputPosition);
  printf("bufferInput: %d\n",test.bufferInputPosition);
  */
  gpsBuffer.defaultBuffer(1024);
  
  setup();
  
  /* //Tests Buffer and getNMEAstr()
  gpsBuffer.push(' ');
  gpsBuffer.push('$');
  gpsBuffer.push('T');
  gpsBuffer.push('E');
  gpsBuffer.push('S');
  gpsBuffer.push('T');
  gpsBuffer.push('\n');
  
  std::string gather="";
  getNMEAstr(&gather);
  printf("%s\n",gather.c_str());*/
  
  
  int overflow = readGPS();
  while( overflow != ERR_SHTAP){
    overflow = readGPS();
  }
  
  //printf(gpsBuffer.buffer);
  
  std::string gather="";
  getNMEAstr(&gather);
  printf("%s\n",gather.c_str());
  gather = "";
  getNMEAstr(&gather);
  printf("%s\n",gather.c_str());
  gather = "";
  getNMEAstr(&gather);
  printf("%s\n",gather.c_str());
  gather = "";
  getNMEAstr(&gather);
  printf("%s\n",gather.c_str());
  gather = "";
  getNMEAstr(&gather);
  printf("%s\n",gather.c_str());
  gather = "";
  getNMEAstr(&gather);
  printf("%s\n",gather.c_str());
  gather = "";
  
  /*setup();
  
  char input;
  while(1){
    read(serialHandle, &input, 1);
    if(input != 0){
      printf("%c\n",input);
    }
  }*/
  
  
}
