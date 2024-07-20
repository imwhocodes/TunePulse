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
// #define SPEED_OPTIMIZED

#ifndef SPEED_OPTIMIZED_PWM_MUX

enum PatternPWM : uint8_t {
  ABCD = 0b11100100,  // Pattern 0: {0, 1, 2, 3}
  ACDB = 0b01111000,  // Pattern 1: {0, 2, 3, 1}
  ADBC = 0b10011100,  // Pattern 2: {0, 3, 1, 2}
  DCAB = 0b01001011   // Pattern 3: {3, 2, 0, 1}
};

class SelectorInterconnectPwm4ch {
 private:
  const int32_t (
      &chABCD)[4];       ///< Reference to input array containing PWM channels.
  const PatternPWM& mode_;  ///< Reference to the current mode.
  int32_t output[4] = {
      INT32_MIN};  ///< Array to store the current output pattern.

 public:
  /**
   * @brief Constructor for SelectorInterconnectPwm4ch.
   * @param mode Reference to the current mode.
   * @param inputArray Reference to the input array containing PWM channels.
   */
  constexpr SelectorInterconnectPwm4ch(const PatternPWM& mode,
                                       const int32_t (&inputArray)[4])
      : mode_(mode), chABCD(inputArray) {}

  /**
   * @brief Updates the output pattern based on the current mode.
   */
  void tick() {
    for (uint8_t i = 0; i < 4; i++) {
      output[i] = chABCD[mode_>>(2*i) & 0b11];
    }
  }

  /**
   * @brief Returns the current PWM channels.
   * @return Reference to the array of current PWM channels.
   */
  constexpr const int32_t (&getPwmChannels() const)[4] { return output; }
};

// enum PatternPWM : uint8_t {
//   ABCD = 0b11100100,  // Pattern 0: {0, 1, 2, 3}
//   ACDB = 0b01111000,  // Pattern 1: {0, 2, 3, 1}
//   ADBC = 0b10011100,  // Pattern 2: {0, 3, 1, 2}
//   DCAB = 0b01001011   // Pattern 3: {3, 2, 0, 1}
// };

// class SelectorInterconnectPwm4ch {
//  private:
//   const int32_t (
//       &chABCD)[4];       ///< Reference to input array containing PWM channels.
//   const uint8_t& mode_;  ///< Reference to the current mode.
//   static constexpr uint32_t pattern =
//       uint32_t(PatternPWM::ABCD)           // ABCD mode
//       | uint32_t(PatternPWM::ACDB) << 8    // ACDB mode
//       | uint32_t(PatternPWM::ADBC) << 16   // ADBC mode
//       | uint32_t(PatternPWM::DCAB) << 24;  // DCAB mode
//   int32_t output[4] = {
//       INT32_MIN};  ///< Array to store the current output pattern.

//  public:
//   /**
//    * @brief Constructor for SelectorInterconnectPwm4ch.
//    * @param mode Reference to the current mode.
//    * @param inputArray Reference to the input array containing PWM channels.
//    */
//   constexpr SelectorInterconnectPwm4ch(const uint8_t& mode,
//                                        const int32_t (&inputArray)[4])
//       : mode_(mode), chABCD(inputArray) {}

//   /**
//    * @brief Updates the output pattern based on the current mode.
//    */
//   void tick() {
//     uint32_t currentPattern = pattern >> 8 * (mode_ % 4);
//     for (uint8_t i = 0; i < 4; ++i) {
//       output[i] = chABCD[currentPattern & 0b11];
//       currentPattern >>= 2;
//     }
//   }

//   /**
//    * @brief Returns the current PWM channels.
//    * @return Reference to the array of current PWM channels.
//    */
//   constexpr const int32_t (&getPwmChannels() const)[4] { return output; }
// };


#else  // SPEED_OPTIMIZED_PWM_MUX

/**
 * @class SelectorInterconnectPwm4ch
 * @brief Class for handling PWM channel re-mapping in a speed optimized manner.
 */
class SelectorInterconnectPwm4ch {
 private:
  const int32_t (
      &chABCD)[4];  ///< Reference to input array containing PWM channels.
  const int32_t*
      pattern[4][4];  ///< Array of pointers to handle different patterns.

  const int32_t** output;  ///< Pointer to the current output pattern.
  const uint8_t& mode_;    ///< Reference to the current mode.

 public:
  /**
   * @brief Constructor for SelectorInterconnectPwm4ch.
   * @param mode Reference to the current mode.
   * @param inputArray Reference to the input array containing PWM channels.
   */
  constexpr SelectorInterconnectPwm4ch(const uint8_t& mode,
                                       const int32_t (&inputArray)[4])
      : mode_(mode),
        chABCD(inputArray),
        pattern{{&chABCD[0], &chABCD[1], &chABCD[2], &chABCD[3]},
                {&chABCD[0], &chABCD[2], &chABCD[3], &chABCD[1]},
                {&chABCD[0], &chABCD[3], &chABCD[1], &chABCD[2]},
                {&chABCD[3], &chABCD[2], &chABCD[0], &chABCD[1]}},
        output(pattern[0]) {}

  /**
   * @brief Updates the output pattern based on the current mode.
   */
  void tick() { output = pattern[mode_ % 4]; }

  /**
   * @brief Returns the current PWM channels.
   * @return Pointer to the array of current PWM channels.
   */
  const int32_t* const* getPwmChannels() const { return output; }
};

#endif  // SPEED_OPTIMIZED_PWM_MUX

#endif  // SELECTOR_INTERCONNECT_PWM_4CH_H