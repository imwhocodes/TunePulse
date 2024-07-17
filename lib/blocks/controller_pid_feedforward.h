/**
 * @file block_pid_feedforward.h
 * @brief Header file for the PID Controller with Feed-Forward Correction
 * @todo Update to work with floats and int64_t
 *
 * This file contains the definition of the PIDController_FF class, which
 * implements a PID (Proportional-Integral-Derivative) controller with
 * an additional feed-forward term. The class is designed to be used in
 * embedded systems for precise control of various processes.
 *
 * The PID controller helps in minimizing the error by adjusting the control
 * input, and the feed-forward term aids in predicting system behavior to
 * improve response time and stability.
 */

#ifndef BLOCK_CONTROLLER_PID_FEEDFORWARD_H
#define BLOCK_CONTROLLER_PID_FEEDFORWARD_H

#include "generic_block.h"

/**
 * @brief PID Controller with Feed-Forward Correction
 * This class implements a PID controller with an additional feed-forward term.
 * @param tick Update the PID controller calculations.
 * @param error An result of the controller's error calculation.
 * @param output An result of the controller's output calculation.
 */
class Controller_PID_FF : public Block {
  BLOCK_INPUT(int32_t, actual);       // The measured process variable
  BLOCK_INPUT(int32_t, reference);    // The reference value (setpoint)
  BLOCK_INPUT(int32_t, feedforward);  // The feed-forward term
  BLOCK_INPUT(int32_t, Kp);     // Proportional coefficient (multiplied by 1000)
  BLOCK_INPUT(int32_t, Ki);     // Integral coefficient (multiplied by 1000)
  BLOCK_INPUT(int32_t, Kd);     // Derivative coefficient (multiplied by 1000)
  BLOCK_INPUT(int32_t, Kff);    // Feed-forward coefficient (multiplied by 1000)
  BLOCK_INPUT(int32_t, limit);  // The output limit
  BLOCK_OUTPUT(int32_t,
               output);          // The variable where the output will be stored
  BLOCK_OUTPUT(int32_t, error);  // The variable where the error will be stored

 private:
  // Internal state variables
  int32_t integral_ = 0;  // Integral term accumulator
  int32_t previous_error_ =
      0;  // Previous error value for derivative calculation

 public:
  /**
   * @brief PIDFFController constructor.
   * @param actual The measured process variable.
   * @param reference The reference value (setpoint).
   * @param feedforward The feed-forward term.
   * @param Kp Proportional coefficient (multiplied by 1000).
   * @param Ki Integral coefficient (multiplied by 1000).
   * @param Kd Derivative coefficient (multiplied by 1000).
   * @param Kff Feed-forward coefficient (multiplied by 1000).
   * @param limit The output limit.
   */
  Controller_PID_FF(const int32_t& actual,
                    const int32_t& reference,
                    const int32_t& feedforward,
                    const int32_t& Kp,
                    const int32_t& Ki,
                    const int32_t& Kd,
                    const int32_t& Kff,
                    const int32_t& limit)
      : actual_(actual),
        reference_(reference),
        feedforward_(feedforward),
        Kp_(Kp),
        Ki_(Ki),
        Kd_(Kd),
        Kff_(Kff),
        limit_(limit) {}

  /**
   * @brief Updates the PID controller calculations.
   * This method computes the new control output based on the PID algorithm
   * and the additional feed-forward term.
   */
  void tick() override;
};

void Controller_PID_FF::tick() {
  // Calculate the error
  error_ = reference_ - actual_;

  // Calculate proportional term
  int32_t P = (Kp_ * error_) / 1000;

  // Calculate integral term with anti-windup
  integral_ += error_;
  integral_ = constrain(integral_, INT32_MIN / 1000, INT32_MAX / 1000);

  int32_t I = (Ki_ * integral_) / 1000;

  // Calculate derivative term
  int32_t D = (Kd_ * (error_ - previous_error_)) / 1000;

  // Calculate feed-forward term
  int32_t FF = (Kff_ * feedforward_) / 1000;

  // Calculate the output and apply the output limit
  int32_t raw_output = P + I + D + FF;
  output_ = constrain(raw_output, -limit_, limit_);

  // Update previous error
  previous_error_ = error_;
}

/*
MACRO FOR INTELLISENSE THAT WILL HELP NOT TO MISPLACE VARIABLES
*/

/**
 * @brief PID Controller with Feed-Forward Correction
 * This class implements a PID controller with an additional feed-forward term.
 * Initializes a PIDController_FF class instance with specified parameters.
 * @param instance_name The unique name for the class instance.
 * @param reference The reference input value (setpoint).
 * @param feedforward The feed-forward input value.
 * @param actual The actual measured input value.
 * @param Kp Proportional control coefficient (multiplied by 1000).
 * @param Ki Integral control coefficient (multiplied by 1000).
 * @param Kd Derivative control coefficient (multiplied by 1000).
 * @param Kff Feed-forward control coefficient (multiplied by 1000).
 * @param output_limit The limit for the output value.
 */
#define INIT_CONTROLLER_PID_FF(instance_name, reference, feedforward, actual, \
                               Kp, Ki, Kd, Kff, output_limit)                 \
  Controller_PID_FF instance_name((actual), (reference), (feedforward), (Kp), \
                                  (Ki), (Kd), (Kff), (output_limit))

#endif  // BLOCK_CONTROLLER_PID_FEEDFORWARD_H