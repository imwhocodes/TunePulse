#ifndef FOC_SYSTEM_SETUP_H
#define FOC_SYSTEM_SETUP_H

#include "blocks_lib.h"

namespace MOTOR_CONTROL {
// Variables for motor and PWM configuration

CurrentControlMode currentMode = VOLTAGE_EST;
MotorType motor_type = STEPPER;

PatternPWM pattern_pwm = ABCD;
ModePWM pwm_mode = ALLIGNED_GND;

VectorAxes2D_I32 voltage_target_mv = {.sin = 0, .cos = 0};     // temporary
VectorPolar2D_I32 current_target_polar = {.ang = 0, .rad = 0};  // current vector
VectorAxes2D_I32 current_target_real = {.sin = 0, .cos = 0};   // temporary

int32_t resistance = 6500;
int16_t pwm_resolution = 3000;

VoltageContainer voltg_container = {.voltg_norm = 580,
                                    .voltg_mv = 12000,
                                    .max_sup_voltage = 69000};

ControllerPIDFF_Setting pid_settgs = {0, 0, 0, 0};  // temp

CurrentVectorPWM currntVectorController(currentMode,
                                        voltage_target_mv,
                                        current_target_polar,
                                        current_target_real,
                                        resistance,
                                        voltg_container,
                                        pid_settgs);

SelectorMotorType motor_sel(motor_type,
                            voltg_container.voltg_norm,
                            currntVectorController.get_voltg_I16(),
                            INT16_MIN);

SelectorInterconnectPwm4ch pwm_mux(pattern_pwm, motor_sel.getPwmChannels());

ModuleDriverPWM pwm(pwm_mode,
                    pwm_resolution,
                    voltg_container.voltg_norm,
                    pwm_mux.getPwmChannels());

void tick() {
  currntVectorController.tick();
  motor_sel.tick();
  pwm_mux.tick();
  pwm.tick();
}

}  // namespace MOTOR_CONTROL

#endif  // FOC_SYSTEM_SETUP_H