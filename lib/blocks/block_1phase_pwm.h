#ifndef BLOCK_DC_PWM_H
#define BLOCK_DC_PWM_H

#include "generic_block.h"

class Block1PhasePWM {
  BLOCK_INPUT(int32_t, voltg_target);

  BLOCK_OUTPUT(int32_t, voltg_a);
  BLOCK_OUTPUT(int32_t, voltg_b);

 private:
  /* data */
 public:
  Block1PhasePWM(const int32_t& voltage) : voltg_target_(voltage) {};

  void tick() {
    voltg_a_ = voltg_target_;
    voltg_b_ = voltg_target_;
    if (voltg_target_ == INT32_MIN)
      return;
    else if (voltg_target_ < 0) {
      voltg_a_ = 0;
      voltg_b_ = abs(voltg_b_);
    } else
      voltg_b_ = 0;
  }
};

#endif