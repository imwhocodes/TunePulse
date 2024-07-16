/**
 * @file block_pid_feedforward.h
 * @brief Header file for the PID Controller with Feed-Forward Correction
 * @todo Update to work with floats and int64_t
 *
 * This file contains the definition of the RegulatorPID_FF class, which
 * implements a PID (Proportional-Integral-Derivative) controller with
 * an additional feed-forward term. The class is designed to be used in
 * embedded systems for precise control of various processes.
 *
 * The PID controller helps in minimizing the error by adjusting the control
 * input, and the feed-forward term aids in predicting system behavior to
 * improve response time and stability.
 *
 * Example usage:
 * @code
 * int32_t reference = 1000;  // Desired setpoint
 * int32_t actual = 0;        // Measured process variable
 * int32_t feedforward = 100; // Feed-forward term
 * int32_t Kp = 500;          // Proportional gain (scaled by 1000)
 * int32_t Ki = 200;          // Integral gain (scaled by 1000)
 * int32_t Kd = 50;           // Derivative gain (scaled by 1000)
 * int32_t Kff = 100;         // Feed-forward gain (scaled by 1000)
 * int32_t limit = 1000;      // Output limit
 * int32_t output = 0;        // Controller output
 * int32_t error = 0;         // Controller error
 *
 * // Initialize the PID controller
 * INIT_REGULATOR_PID_FF(my_pid, reference, feedforward, actual, Kp, Ki, Kd,
 * Kff, limit, output, error);
 *
 * // Update the controller
 * my_pid.tick();
 * @endcode
 */

#ifndef BLOCK_PID_FEEDFORWARD_H
#define BLOCK_PID_FEEDFORWARD_H

#include "generic_block.h"

/**
 * @brief PID Controller with Feed-Forward Correction
 * This class implements a PID controller with an additional feed-forward term.
 * @param ref The reference value.
 * @param est The in_estimated value (feed-forward term).
 * @param act The in_actual measured value.
 * @param err The variable where the error will be stored.
 * @param out The variable where the output will be stored.
 * @param Kp Proportional coefficient (multiplied by 1000).
 * @param Ki Integral coefficient (multiplied by 1000).
 * @param Kd Derivative coefficient (multiplied by 1000).
 * @param Kff Feed-forward coefficient (multiplied by 1000).
 * @param out_limit The output limit.
 */
class RegulatorPID_FF : public Block {
  BLOCK_INPUT(int32_t, actual);
  BLOCK_INPUT(int32_t, reference);
  BLOCK_INPUT(int32_t, feedforward);
  BLOCK_INPUT(int32_t, Kp);
  BLOCK_INPUT(int32_t, Ki);
  BLOCK_INPUT(int32_t, Kd);
  BLOCK_INPUT(int32_t, Kff);
  BLOCK_INPUT(int32_t, limit);
  BLOCK_OUTPUT(int32_t, output);
  BLOCK_OUTPUT(int32_t, error);

 private:
  // Internal state variables
  int32_t integral_ = 0;
  int32_t previous_error_ = 0;

 public:
  /**
   * @brief PIDFFController constructor.
   * @param ref The reference value (setpoint).
   * @param est The in_estimated value (feed-forward term).
   * @param act The in_actual measured value.
   * @param err The variable where the error will be stored.
   * @param out The variable where the output will be stored.
   * @param Kp Proportional coefficient (multiplied by 1000).
   * @param Ki Integral coefficient (multiplied by 1000).
   * @param Kd Derivative coefficient (multiplied by 1000).
   * @param Kff Feed-forward coefficient (multiplied by 1000).
   * @param out_limit The output limit.
   */
  RegulatorPID_FF(const int32_t& in_actual,
                  const int32_t& in_reference,
                  const int32_t& in_feedforward,
                  const int32_t& in_kp,
                  const int32_t& in_ki,
                  const int32_t& in_kd,
                  const int32_t& in_kff,
                  const int32_t& in_limit)
      : actual_(in_actual),
        reference_(in_reference),
        feedforward_(in_feedforward),
        Kp_(in_kp),
        Ki_(in_ki),
        Kd_(in_kd),
        Kff_(in_kff),
        limit_(in_limit) {}

  void tick() override {
    // Calculate the error
    error_ = reference_ - actual_;

    // Calculate proportional term
    int32_t P = (Kp_ * error_) / 1000;

    // Calculate integral term with anti-windup
    integral_ += error;
    output_ = constrain(integral_, INT32_MIN / 1000, INT32_MAX / 1000);

    int32_t I = (Ki_ * integral_) / 1000;

    // Calculate derivative term
    int32_t D = (Kd_ * (error_ - previous_error_)) / 1000;

    // Calculate feed-forward term
    int32_t FF = (Kff_ * feedforward_) / 1000;

    // Calculate the output and apply the output limit
    int32_t raw_output = P + I + D + FF;
    output_ = constrain(raw_output, -limit_, +limit_);

    // Update previous error
    previous_error_ = error;
  }
};

/*
MACRO FOR INTELLISENSE THAT WILL HELP NOT TO MISPLACE VARIABLES
*/

/**
 * @brief PID Controller with Feed-Forward Correction
 * This class implements a PID controller with an additional feed-forward term.
 * Initializes a RegulatorPID_FF class instance with specified parameters.
 * @param instance_name The unique name for the class instance.
 * @param in_reference The reference input value (setpoint).
 * @param in_feedforward The feed-forward input value.
 * @param in_actual The actual measured input value.
 * @param in_Kp Proportional control coefficient (multiplied by 1000).
 * @param in_Ki Integral control coefficient (multiplied by 1000).
 * @param in_Kd Derivative control coefficient (multiplied by 1000).
 * @param in_Kff Feed-forward control coefficient (multiplied by 1000).
 * @param in_output_limit The limit for the output value.
 * @param out_value The variable where the output value will be stored.
 * @param out_error The variable where the error value will be stored.
 */
#define INIT_REGULATOR_PID_FF(instance_name, in_reference, in_feedforward,     \
                              in_actual, in_Kp, in_Ki, in_Kd, in_Kff,          \
                              in_output_limit)           \
  RegulatorPID_FF instance_name((in_actual), (in_reference), (in_feedforward), \
                                (in_Kp), (in_Ki), (in_Kd), (in_Kff),           \
                                (in_output_limit))

#endif  // BLOCK_PID_FEEDFORWARD_H