#ifndef MODULE_PWM_DRIVER_H
#define MODULE_PWM_DRIVER_H

#include "generic_block.h"

enum ModePWM : uint8_t { ALLIGNED_GND = 0, ALLIGNED_VCC = 1 };

class ModuleDriverPWM {
  BLOCK_INPUT(ModePWM, mode);
  const int32_t (&ch1234_)[4];
  BLOCK_INPUT(int32_t, sup_voltg);
  BLOCK_INPUT(int32_t, pwm_res);
  int32_t duty[4] = {0};
  bool enb[4] = {0};

 private:
  /* data */
 public:
  ModuleDriverPWM(const ModePWM& mode,
                  const int32_t& pwm_resolution,
                  const int32_t& sup_voltage,
                  const int32_t (&ch1234)[4])
      : mode_(mode),
        ch1234_(ch1234),
        sup_voltg_(sup_voltage),
        pwm_res_(pwm_resolution) {}

  void tick() {
    for (uint8_t i = 0; i < 4; i++) {
      enb[i] = {bool(ch1234_[i] != INT32_MIN)};

      duty[i] = (ch1234_[i] * pwm_res_) / sup_voltg_;
      if (duty[i] > 0) duty[i]+=15;
      if (duty[i] > pwm_res_)
        duty[i] = pwm_res_;
      // if (mode_ == ALLIGNED_VCC)
      //   duty[i] = pwm_res_ - duty[i];
    }
  }

  constexpr const int32_t (&getPwmChannels() const)[4] { return duty; }
};

#endif  // MODULE_PWM_DRIVER_H