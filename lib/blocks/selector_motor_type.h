#ifndef SELECTOR_MOTOR_TYPE_H
#define SELECTOR_MOTOR_TYPE_H

#include "block_1phase_pwm.h"
#include "block_3phase_pwm.h"
#include "generic_block.h"

enum MotorType : uint8_t { DC = 0, BLDC, STEPPER };

class SelectorMotorType {
  BLOCK_INPUT(int32_t, voltg_alpha);
  BLOCK_INPUT(int32_t, voltg_beta);
  BLOCK_INPUT(int32_t, voltg_sup);
  BLOCK_INPUT(MotorType, mode);

  int32_t ChABCD[4] = {INT32_MIN};

 private:
  Block3PhasePWM bldc;
  Block1PhasePWM dc1;
  Block1PhasePWM dc2;

 private:
  /* data */
 public:
  SelectorMotorType(const MotorType& mode,
                    const int32_t& sup,
                    const int32_t& alpha,
                    const int32_t& beta)
      : mode_(mode),
        voltg_sup_(sup),
        voltg_alpha_(alpha),
        voltg_beta_(beta),
        bldc(sup, alpha, beta),
        dc1(alpha),
        dc2(beta) {}

  void tick() {
    switch (mode_) {
      case (BLDC):
        bldc.tick();
        ChABCD[0] = bldc.get_voltg_a();
        ChABCD[1] = bldc.get_voltg_b();
        ChABCD[2] = bldc.get_voltg_c();
        ChABCD[3] = 0;
        break;

      case (DC):
        dc1.tick();
        ChABCD[0] = dc1.get_voltg_a();
        ChABCD[1] = dc1.get_voltg_b();
        ChABCD[2] = INT32_MIN;
        ChABCD[3] = INT32_MIN;
        break;
      case (STEPPER):
        dc1.tick();
        dc2.tick();
        ChABCD[0] = dc1.get_voltg_a();
        ChABCD[1] = dc1.get_voltg_b();
        ChABCD[2] = dc2.get_voltg_a();
        ChABCD[3] = dc2.get_voltg_b();
        break;

      default:
        ChABCD[0] = INT32_MIN;
        ChABCD[1] = INT32_MIN;
        ChABCD[2] = INT32_MIN;
        ChABCD[3] = INT32_MIN;
        break;
    }
  }

  constexpr const int32_t (&getPwmChannels() const)[4] { return ChABCD; }
};

#endif  // SELECTOR_MOTOR_TYPE_H