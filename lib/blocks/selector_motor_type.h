/**
 * @file selector_motor_type.h
 * @brief Header file for motor type selector class.
 */

#ifndef SELECTOR_MOTOR_TYPE_H
#define SELECTOR_MOTOR_TYPE_H

#include <inttypes.h>
#include "generic_block.h"

/**
 * @class SelectorMotorType
 * @brief Class to handle different types of motor controls.
 */
class SelectorMotorType {
  const VectorAxes2D_I16& voltg_;  // Input for alpha component of voltage
  const int16_t& voltg_sup_;       // Input for supply voltage
  const int16_t& voltg_brak_;      // Input for brake voltage
  const MotorType& mode_;          // Input for motor type mode

 protected:
  int16_t ChABCD[4];  // Array to store voltages for four channels

  /**
   * @enum phase_index
   * @brief Enumeration for channel indices.
   */
  enum phase_index : uint8_t {
    ChannelA = 0,  // Channel sin
    ChannelB = 1,  // Channel B
    ChannelC = 2,  // Channel C
    ChannelD = 3   // Channel D
  };

  /**
   * @brief Function to calculate coil voltages.
   * @param voltg_ref_ Reference voltage.
   * @param voltg_out_1 Output voltage 1.
   * @param voltg_out_2 Output voltage 2.
   */
  void math_coil(const int16_t& voltg_ref_,
                 int16_t& voltg_out_1,
                 int16_t& voltg_out_2);

  /**
   * @brief Function to handle one-phase motor control.
   */
  void tick1PHASE();

  /**
   * @brief Function to handle two-phase motor control.
   */
  void tick2PHASE();

  /**
   * @brief Function to control 3Phase 3Wire motor with SVPWM algorithm.
   */
  void tick3PHASE();

 public:
  /**
   * @brief Constructor for SelectorMotorType.
   * @param mode Reference to the motor type mode.
   * @param sup Reference to the supply voltage.
   * @param alpha Reference to the alpha component of the voltage.
   * @param beta Reference to the beta component of the voltage.
   * @param brake Reference to the brake voltage.
   */
  SelectorMotorType(const MotorType& mode,
                    const int16_t& sup,
                    const VectorAxes2D_I16& voltg_norm,
                    const int16_t& brake)
      : mode_(mode), voltg_sup_(sup), voltg_(voltg_norm), voltg_brak_(brake) {}

  /**
   * @brief Function to update motor control based on mode.
   */
  void tick() {
    switch (mode_) {
      case (DC):
        tick1PHASE();
        break;
      case (STEPPER):
        tick2PHASE();
        break;
      case (BLDC):
        tick3PHASE();
        break;
      default:
        ChABCD[0] = INT16_MIN;
        ChABCD[1] = INT16_MIN;
        ChABCD[2] = INT16_MIN;
        ChABCD[3] = INT16_MIN;
        break;
    }
  }

  /**
   * @brief Function to get PWM channel values.
   * @return Reference to array of PWM channel values.
   */
  constexpr const int16_t (&getPwmChannels() const)[4] { return ChABCD; }
};

void SelectorMotorType::math_coil(const int16_t& voltg_ref_,
                                  int16_t& voltg_out_1,
                                  int16_t& voltg_out_2) {
  // Set the default voltage for both outputs
  voltg_out_1 = voltg_ref_;
  voltg_out_2 = voltg_ref_;

  // If the reference voltage is equal to the disable state, do not change
  // anything
  if (voltg_ref_ == INT16_MIN)
    return;
  // If the reference voltage is negative, set to reverse drive mode
  else if (voltg_ref_ < 0) {
    voltg_out_1 = 0;
    voltg_out_2 = abs(voltg_out_2);
  }
  // If the reference voltage is zero or positive, set to direct drive mode
  else
    voltg_out_2 = 0;

  //  Total operations: Min 4, Max 6
}

void SelectorMotorType::tick1PHASE() {
  SelectorMotorType::math_coil(voltg_.sin, ChABCD[ChannelA], ChABCD[ChannelB]);
  ChABCD[ChannelC] = voltg_brak_;
  ChABCD[ChannelD] = voltg_brak_;
}

void SelectorMotorType::tick2PHASE() {
  SelectorMotorType::math_coil(voltg_.sin, ChABCD[ChannelA], ChABCD[ChannelB]);
  SelectorMotorType::math_coil(voltg_.cos, ChABCD[ChannelC], ChABCD[ChannelD]);
}

void SelectorMotorType::tick3PHASE() {
  // Precalculated sqrt(3)/2 scaled to uint16_t (sqrt(3)/2 * 2^16)
  static constexpr int32_t SQRT3DIV2 = 56755;

  // Convert beta voltage component to a scaled value using SQRT3DIV2
  int32_t voltg_beta_sqrt3_div2_ = (SQRT3DIV2 * voltg_.cos) >> 16;

  //////// Clarke Transformation ///////////////
  // Set phase A voltage to the alpha component
  ChABCD[ChannelA] = voltg_.sin;

  // Calculate phase B voltage: -1/2 * V_alpha + sqrt(3)/2 * V_beta
  ChABCD[ChannelB] = -voltg_.sin / 2 + voltg_beta_sqrt3_div2_;

  // Calculate phase C voltage: -1/2 * V_alpha - sqrt(3)/2 * V_beta
  ChABCD[ChannelC] = -voltg_.sin / 2 - voltg_beta_sqrt3_div2_;

  // Space Vector Pulse Width Modulation (SVPWM) Algorithm
  int16_t voltg_min_ =
      min(ChABCD[ChannelA], min(ChABCD[ChannelB], ChABCD[ChannelC]));
  int16_t voltg_max_ =
      max(ChABCD[ChannelA], max(ChABCD[ChannelB], ChABCD[ChannelC]));
  int16_t voltg_ref_ = ((voltg_sup_ - voltg_max_) - voltg_min_) >> 1;

  // Shift all phase voltages by voltage_ref_
  ChABCD[ChannelA] += voltg_ref_;
  ChABCD[ChannelB] += voltg_ref_;
  ChABCD[ChannelC] += voltg_ref_;

  // Set unused phase to brake (optional)
  ChABCD[ChannelD] = voltg_brak_;

  //  Total Operations:  17
}

#endif  // SELECTOR_MOTOR_TYPE_H