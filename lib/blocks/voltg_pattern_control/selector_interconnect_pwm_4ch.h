/**
 * @file selector_interconnect_pwm_4ch.h
 * @brief Header file for the SelectorInterconnectPwm4ch class.
 *
 * This class handles the re-mapping of PWM channels for universal motor
 * connection without being bound to phase sequence.
 */

#ifndef SELECTOR_INTERCONNECT_PWM_4CH_H
#define SELECTOR_INTERCONNECT_PWM_4CH_H

#include "..\data_types.h"

class SelectorInterconnectPwm4ch {
 private:
  const VoltgChannelslNormlzd& chABCD_;  // Ref to array containing ABCD channels
  const PatternPWM& mode_;               // Reference to the current mode
  VoltgChannelslNormlzd output = {
      INT16_MIN};  // Array to store the current output pattern.

 public:
  /**
   * @brief Constructor for SelectorInterconnectPwm4ch.
   * @param mode Reference to the current mode.
   * @param inputArray Reference to the input array containing PWM channels.
   */
  constexpr SelectorInterconnectPwm4ch(const PatternPWM& mode,
                                       const VoltgChannelslNormlzd& chABCD)
      : mode_(mode), chABCD_(chABCD) {}

  /**
   * @brief Updates the output pattern based on the current mode.
   */
  void tick() {
    for (uint8_t i = 0; i < 4; i++)
      output[i] = chABCD_[mode_ >> (i * 2) & 0b11];
  }

  /**
   * @brief Returns the current PWM channels.
   * @return Reference to the array of current PWM channels.
   */
  const VoltgChannelslNormlzd& getPwmChannels() const { return output; }
};

#endif  // SELECTOR_INTERCONNECT_PWM_4CH_H