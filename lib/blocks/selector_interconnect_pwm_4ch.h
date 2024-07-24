/**
 * @file selector_interconnect_pwm_4ch.h
 * @brief Header file for the SelectorInterconnectPwm4ch class.
 *
 * This class handles the re-mapping of PWM channels for universal motor
 * connection without being bound to phase sequence.
 */

#ifndef SELECTOR_INTERCONNECT_PWM_4CH_H
#define SELECTOR_INTERCONNECT_PWM_4CH_H

#include "generic_block.h"

// Uncomment this line to optimize for speed - not recommended
// #define SPEED_OPTIMIZED_PWM_MUX


#ifndef SPEED_OPTIMIZED_PWM_MUX

class SelectorInterconnectPwm4ch {
 private:
  const int16_t (&chABCD_)[4];  // Ref to array containing ABCD PWM channels.
  const PatternPWM& mode_;      // Reference to the current mode
  int16_t output[4] = {
      INT16_MIN};  // Array to store the current output pattern.

 public:
  /**
   * @brief Constructor for SelectorInterconnectPwm4ch.
   * @param mode Reference to the current mode.
   * @param inputArray Reference to the input array containing PWM channels.
   */
  constexpr SelectorInterconnectPwm4ch(const PatternPWM& mode,
                                       const int16_t (&chABCD)[4])
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
  constexpr const int16_t (&getPwmChannels() const)[4] { return output; }
};

#else  // SPEED_OPTIMIZED_PWM_MUX

// class SelectorInterconnectPwm4ch {
//  private:
//   const int16_t (&chABCD_)[4];  // Ref to array containing ABCD PWM channels.
//   const PatternPWM& mode_;     // Reference to the current mode
//   PatternPWM previous_mode_ = ABCD;  // Reference to the current mode
//   const int16_t* pattern[4];  // Array to store the current output pattern.

//  public:
//   /**
//    * @brief Constructor for SelectorInterconnectPwm4ch.
//    * @param mode Reference to the current mode.
//    * @param inputArray Reference to the input array containing PWM channels.
//    */
//   constexpr SelectorInterconnectPwm4ch(const PatternPWM& mode,
//                                        const int16_t (&inputArray)[4])
//       : mode_(mode),
//         chABCD_(inputArray),
//         pattern(
//             {&inputArray[0], &inputArray[1], &inputArray[2], &inputArray[3]})
//             {}

//   /**
//    * @brief Updates the output pattern based on the current mode.
//    */
//   void tick() {
//     if (previous_mode_ != mode_) {
//       for (int8_t i = 0; i != 4; i++)
//         pattern[i] = &chABCD_[mode_ >> (i * 2) & 0b11];
//       previous_mode_ = mode_;
//     }
//   }

//   /**
//    * @brief Returns the current PWM channels.
//    * @return Reference to the array of current PWM channels.
//    */
//   const int16_t* const (&getPwmChannels() const)[4] { return pattern; }
// };

/**
 * @class SelectorInterconnectPwm4ch
 * @brief Class for handling PWM channel re-mapping in a speed optimized manner.
 */
class SelectorInterconnectPwm4ch {
 private:
  const int16_t (
      &chABCD_)[4];  ///< Reference to input array containing PWM channels.
  const int16_t*
      pattern[4][4];  ///< Array of pointers to handle different patterns.

  const int16_t** output;  ///< Pointer to the current output pattern.
  const uint8_t& mode_;    ///< Reference to the current mode.

 public:
  /**
   * @brief Constructor for SelectorInterconnectPwm4ch.
   * @param mode Reference to the current mode.
   * @param inputArray Reference to the input array containing PWM channels.
   */
  constexpr SelectorInterconnectPwm4ch(const uint8_t& mode,
                                       const int16_t (&inputArray)[4])
      : mode_(mode),
        chABCD_(inputArray),
        pattern{{&chABCD_[0], &chABCD_[1], &chABCD_[2], &chABCD_[3]},
                {&chABCD_[0], &chABCD_[2], &chABCD_[3], &chABCD_[1]},
                {&chABCD_[0], &chABCD_[3], &chABCD_[1], &chABCD_[2]},
                {&chABCD_[3], &chABCD_[2], &chABCD_[0], &chABCD_[1]}},
        output(pattern[0]) {}

  /**
   * @brief Updates the output pattern based on the current mode.
   */
  void tick() { output = pattern[mode_ % 4]; }

  /**
   * @brief Returns the current PWM channels.
   * @return Pointer to the array of current PWM channels.
   */
  const int16_t* const* getPwmChannels() const { return output; }
};

#endif  // SPEED_OPTIMIZED_PWM_MUX

#endif  // SELECTOR_INTERCONNECT_PWM_4CH_H