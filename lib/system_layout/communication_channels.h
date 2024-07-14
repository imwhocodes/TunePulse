#ifndef COMMUNICATION_CHANNELS_H
#define COMMUNICATION_CHANNELS_H

#include "..\tools\macros\communication_channel_macros.h"
#include "float.h"
#include "inttypes.h"

namespace TUNEPULSE_CHANNELS {
// Adjusts dq voltage to compensate motor Back EMF due to rotation
CHANNEL_INIT_DEFAULT(int32_t, SPEED_INST, 0)
CHANNEL_INIT_DEFAULT(int32_t, BEMF_D, 0)
CHANNEL_INIT_DEFAULT(int32_t, MOTOR_KV, 0)
CHANNEL_INIT_DEFAULT(int32_t, PHASE_SHIFT, 0)
CHANNEL_INIT_DEFAULT(int32_t, ROTOR_ANGLE, 0)
CHANNEL_INIT_DEFAULT(int32_t, BEMF_Q, 0)

// Collects and normalizes data from encoder considering its resolution
CHANNEL_INIT_DEFAULT(int32_t, ANGLE_RESOL, 0)
CHANNEL_INIT_DEFAULT(int32_t, ANGLE_RAW, 0)

// Collects and normalizes MCU temperature
CHANNEL_INIT_DEFAULT(int32_t, TEMP, 0)

// Collects and normalizes supply voltage
CHANNEL_INIT_DEFAULT(int32_t, VOLTG_SUP, 0)

// Collects and normalizes motor power consumption
CHANNEL_INIT_DEFAULT(int32_t, MOTOR_POWER, 0)

// Control maximum current applied to motor to prevent overheating
CHANNEL_INIT_DEFAULT(int32_t, CURNT_CH_A, 0)
CHANNEL_INIT_DEFAULT(int32_t, CURNT_CH_B, 0)
CHANNEL_INIT_DEFAULT(int32_t, CURNT_CH_C, 0)
CHANNEL_INIT_DEFAULT(int32_t, CURNT_CH_D, 0)
CHANNEL_INIT_DEFAULT(int32_t, CURNT_LIMIT, 0)
CHANNEL_INIT_DEFAULT(int32_t, TEMP_THRSHLD, 0)
CHANNEL_INIT_DEFAULT(int32_t, MOTOR_CURNT, 0)

// Estimates Mechanical Angle per Electrical Period and Frequency
CHANNEL_INIT_DEFAULT(int64_t, POSITION_RAW, 0)
CHANNEL_INIT_DEFAULT(int64_t, ROTOR_OFST, 0)
CHANNEL_INIT_DEFAULT(int32_t, POLE_PERIOD, 0)

// Absolute position encoder handler (angle + full rotation index)
CHANNEL_INIT_DEFAULT(int64_t, POSITION_OFST, 0)
CHANNEL_INIT_DEFAULT(int64_t, POSITION_INST, 0)

// Collects and normalizes current for each channel
CHANNEL_INIT_DEFAULT(int32_t, CURNT_CH_1, 0)
CHANNEL_INIT_DEFAULT(int32_t, CURNT_CH_2, 0)
CHANNEL_INIT_DEFAULT(int32_t, CURNT_CH_3, 0)
CHANNEL_INIT_DEFAULT(int32_t, CURNT_CH_4, 0)

// Handles and applies electric angle and current to step-dir driver
CHANNEL_INIT_DEFAULT(bool, DRIVER_OFF, false)
CHANNEL_INIT_DEFAULT(int32_t, PHASE_CURNT, 0)
CHANNEL_INIT_DEFAULT(int32_t, PHASE_ANGLE, 0)

// Converts BldcABC → αβ Current
CHANNEL_INIT_DEFAULT(int32_t, CURNT_COS, 0)
CHANNEL_INIT_DEFAULT(int32_t, CURNT_SIN, 0)

// Converts DcStepAB → αβ Current
// (same variables as above: CURNT_COS and CURNT_SIN)

// Corrects ABCD⇄1234 current channel connection for universal compatibility
CHANNEL_INIT_DEFAULT(int32_t, CH_MUX_MODE, 0)
CHANNEL_INIT_DEFAULT(int32_t, MUX_SUPPORT, 0)

// Converts required current vector into phase position control
CHANNEL_INIT_DEFAULT(int32_t, CURNT_SIN_REF, 0)
CHANNEL_INIT_DEFAULT(int32_t, CURNT_COS_REF, 0)

// Regulates sin component of current to be exact as desired without saturation
CHANNEL_INIT_DEFAULT(int32_t, VOLTG_SIN, 0)

// Regulates cos component of current to be exact as desired without saturation
CHANNEL_INIT_DEFAULT(int32_t, VOLTG_COS, 0)

// Estimates sincos voltage to get desired current without saturation
CHANNEL_INIT_DEFAULT(int32_t, MOTOR_RESIST, 0)

// Converts ABCD voltage to required PWM duty with safety constraining
CHANNEL_INIT_DEFAULT(int32_t, VOLTG_Q_ADJ, 0)
CHANNEL_INIT_DEFAULT(int32_t, VOLTG_D_ADJ, 0)

// Calculates Sine and Cosine based on given angle
CHANNEL_INIT_DEFAULT(int32_t, ROTOR_SIN, 0)
CHANNEL_INIT_DEFAULT(int32_t, ROTOR_COS, 0)

// Converts dq → αβ Voltage using encoder data
CHANNEL_INIT_DEFAULT(int32_t, VOLTG_SIN_REF, 0)
CHANNEL_INIT_DEFAULT(int32_t, VOLTG_D_ADJ, 0)
CHANNEL_INIT_DEFAULT(int32_t, VOLTG_COS_REF, 0)

// Converts αβ Current → dq Current using encoder data
CHANNEL_INIT_DEFAULT(int32_t, CURNT_Q_REAL, 0)
CHANNEL_INIT_DEFAULT(int32_t, CURNT_D_REAL, 0)

// Handles and applies PWM values to the driver
CHANNEL_INIT_DEFAULT(int32_t, VOLTG_CH_1, 0)
CHANNEL_INIT_DEFAULT(int32_t, VOLTG_CH_2, 0)
CHANNEL_INIT_DEFAULT(int32_t, VOLTG_CH_3, 0)
CHANNEL_INIT_DEFAULT(int32_t, VOLTG_CH_4, 0)

// Corrects ABCD⇄1234 voltage channel connection for universal compatibility
CHANNEL_INIT_DEFAULT(int32_t, VOLTG_CH_A, 0)
CHANNEL_INIT_DEFAULT(int32_t, VOLTG_CH_B, 0)
CHANNEL_INIT_DEFAULT(int32_t, VOLTG_CH_C, 0)
CHANNEL_INIT_DEFAULT(int32_t, VOLTG_CH_D, 0)


// Regulates electric angle in motor to get desired rotor pos without skipping
// steps
CHANNEL_INIT_DEFAULT(int64_t, POSITION_ERR, 0)
CHANNEL_INIT_DEFAULT(int64_t, POSITION_EXPT, 0)

// Calculates current required to achieve desired torque without overloading
CHANNEL_INIT_DEFAULT(int32_t, TORQUE_EST, 0)
CHANNEL_INIT_DEFAULT(int32_t, MOTOR_KT, 0)
CHANNEL_INIT_DEFAULT(int32_t, TORQUE_ERR, 0)
CHANNEL_INIT_DEFAULT(int32_t, CURNT_REF, 0)

// Regulates torque required to achieve desired speed without saturation
CHANNEL_INIT_DEFAULT(int32_t, SPEED_EST, 0)
CHANNEL_INIT_DEFAULT(int32_t, TORQUE_LIMIT, 0)
CHANNEL_INIT_DEFAULT(int32_t, SPEED_REF, 0)
CHANNEL_INIT_DEFAULT(int32_t, SPEED_ERR, 0)

// Corrects dq current proportion to get desired torque at required current
CHANNEL_INIT_DEFAULT(int32_t, CURNT_Q_REF, 0)
CHANNEL_INIT_DEFAULT(int32_t, CURNT_D_REF, 0)
CHANNEL_INIT_DEFAULT(int32_t, CURNT_STEP, 0)
CHANNEL_INIT_DEFAULT(int32_t, CURNT_IDLE, 0)

// Converts required motion profile to instant accel/speed/position
CHANNEL_INIT_DEFAULT(int32_t, SPEED_TARG, 0)
CHANNEL_INIT_DEFAULT(int64_t, TRANSL_TARG, 0)
CHANNEL_INIT_DEFAULT(int32_t, ACCEL_TARG, 0)
CHANNEL_INIT_DEFAULT(int64_t, TIME, 0)
CHANNEL_INIT_DEFAULT(int64_t, POSITION_REF, 0)
CHANNEL_INIT_DEFAULT(int32_t, ACCEL_REF, 0)
CHANNEL_INIT_DEFAULT(int32_t, JERK_TARG, 0)

// Regulates torque required to achieve desired acceleration
CHANNEL_INIT_DEFAULT(int32_t, MOTOR_INERTIA, 0)

// Regulates speed required to achieve desired position without saturation
CHANNEL_INIT_DEFAULT(int64_t, POSITION_REF, 0)

// Additional variables from the diagram:
CHANNEL_INIT_DEFAULT(int32_t, MSTEP_SIZE, 0)
CHANNEL_INIT_DEFAULT(int32_t, PWM_RESOL, 0)
CHANNEL_INIT_DEFAULT(int32_t, CURNT_RUN, 0)
CHANNEL_INIT_DEFAULT(int32_t, MOTOR_INERTIA, 0)
CHANNEL_INIT_DEFAULT(int32_t, MOTOR_KT, 0)
CHANNEL_INIT_DEFAULT(int32_t, MOTOR_KV, 0)
CHANNEL_INIT_DEFAULT(int32_t, MOTOR_RESIST, 0)
CHANNEL_INIT_DEFAULT(int32_t, MOTOR_CURNT, 0)
CHANNEL_INIT_DEFAULT(int64_t, ROTOR_OFST, 0)
}  // namespace TUNEPULSE_CHANNELS

#endif  // COMMUNICATION_CHANNELS_H
