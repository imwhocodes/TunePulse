#ifndef REGULATOR_PID_FEEDFORWARD_H
#define REGULATOR_PID_FEEDFORWARD_H

#include <Arduino.h>


#include "generic_modules.h"

class PID_FF : public ComputeModule {
 private:
  // References to the initial variables
  MODULE_CHANNEL_INIT(int32_t, in, CHANNEL_INPUT);
  MODULE_CHANNEL_INIT(int32_t, out, CHANNEL_OUTPUT);
  
 public:
  PID_FF(int32_t& _in, int32_t &_out):  in_link(_in), out_link(_out)  {}

  void read() override {
    SerialUSB.print("Read value ");
    MODULE_CHANNEL_READ(in);
    SerialUSB.println(in);
    
  }

  void compute() override {
    SerialUSB.println(in);
    out = 2*in;
  }

  void write() override {
    MODULE_CHANNEL_WRITE(out);
  }
};
#endif  // REGULATOR_PID_FEEDFORWARD_H