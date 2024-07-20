/**
 * @file block_3phase_pwm.h
 * @brief Header file for the Block3PhasePWM class, implementing a 3-phase PWM
 * block.
 */

#ifndef BLOCK_3PHASE_PWM_H
#define BLOCK_3PHASE_PWM_H

#include "generic_block.h"
#include "Arduino.h"

/**
 * @class Block3PhasePWM
 * @brief Class for handling 3-phase PWM control using integer implementation of
 * SVPWM algorithm.
 */
class Block3PhasePWM {
  BLOCK_INPUT(int32_t, voltg_sup);   /**< Input voltage supply. */
  BLOCK_INPUT(int32_t, voltg_alpha); /**< Input voltage alpha component. */
  BLOCK_INPUT(int32_t, voltg_beta);  /**< Input voltage beta component. */

  BLOCK_OUTPUT(int32_t, voltg_a); /**< Output voltage phase A. */
  BLOCK_OUTPUT(int32_t, voltg_b); /**< Output voltage phase B. */
  BLOCK_OUTPUT(int32_t, voltg_c); /**< Output voltage phase C. */

 private:
  /**
   * @brief Define sqrt(3)/2 as a uint32_t constant, scaled to fit in the range
   * of uint32_t. Approximately (sqrt(3)/2 * 2^32).
   */
  static constexpr int64_t SQRT3DIV2 = 3719550786;

 public:
  /**
   * @brief Constructor for Block3PhasePWM.
   * @param sup Reference to the supply voltage.
   * @param alpha Reference to the alpha component of the voltage.
   * @param beta Reference to the beta component of the voltage.
   */
  Block3PhasePWM(const int32_t& sup, const int32_t& alpha, const int32_t& beta)
      : voltg_sup_(sup), voltg_alpha_(alpha), voltg_beta_(beta) {}

  /**
   * @brief Method to update the PWM values based on the input voltages using
   * SVPWM algorithm.
   */
  void tick() {
    // Clarke Transformation
    // Convert beta voltage component to a scaled value using SQRT3DIV2
    int32_t voltg_beta_sqrt3_div2_ =
        (int32_t)((SQRT3DIV2 * voltg_beta_) >> 32);

    // Set phase A voltage to the alpha component
    voltg_a_ = voltg_alpha_;

    // Calculate phase B voltage: -1/2 * V_alpha + sqrt(3)/2 * V_beta
    voltg_b_ = -voltg_alpha_ / 2 + voltg_beta_sqrt3_div2_;

    // Calculate phase C voltage: -1/2 * V_alpha - sqrt(3)/2 * V_beta
    voltg_c_ = -voltg_alpha_ / 2 - voltg_beta_sqrt3_div2_;

    // Space Vector Pulse Width Modulation (SVPWM) Algorithm
    int32_t voltg_min_ = min(voltg_a_, min(voltg_b_, voltg_c_));
    int32_t voltg_max_ = max(voltg_a_, max(voltg_b_, voltg_c_));
    int32_t voltg_ref_ = ((voltg_sup_ - voltg_max_) - voltg_min_) >> 1;

    // Shift all phase voltages by voltage_ref_
    voltg_a_ += voltg_ref_;
    voltg_b_ += voltg_ref_;
    voltg_c_ += voltg_ref_;
  }
};

#endif  // BLOCK_3PHASE_PWM_H