//
// EagleAeroSport
//
// B.Davis
//
//////////////////
// this is the structure which contains the data which is SHARED not replicated
// between the parallel threads of execution.

typedef struct shared_vars {
  bool gDone;
  bool gAccelEnabled;
  bool gPressEnabled;
  bool gTempEnabled;
  unsigned int iLoopPause;
} shared_vars_t;
