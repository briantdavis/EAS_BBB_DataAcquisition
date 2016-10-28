
#include <cstdint>

#include "eas_daq_pack.h"

#ifndef PCA9544MUX_H_
#define PCA9544MUX_H_

#define PCA9544_000_ADDR    0x70

class PCA9544Mux {
public:
typedef enum muxChan { UNDEF = -1,
                      CH_0, CH_1, CH_2, CH_3,
                      DISABLED} muxChan_t;

private:
  int i2cBus;       // processor bus #
  int i2cAddress;   // i2c bus addres (7-bit)
  int i2cHandle;    // Handle returned from open()
  int opResult;

  muxChan chanSelected;
 
public:
 //
 // Class Member Fn
 //
 

 	PCA9544Mux(int, int);
 	virtual ~PCA9544Mux();
  
  //
  // Mutator
  void blank();
  int selectChan(muxChan_t);
  
  int logPartASensorID(std::ofstream &);
  
};

#endif