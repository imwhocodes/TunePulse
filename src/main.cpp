#include <Arduino.h>
#include "blocks_lib.h"
#include "timerPWM.h"

// Variables for motor and PWM configuration
MotorType motor_type = STEPPER;
PatternPWM pattern_pwm = ABCD;
ModePWM pwm_mode = ALLIGNED_GND;

int16_t sup_vltg = 10000;           // not used yet
int16_t pwm_resolution = 4000;  // not used yet

int32_t motor_voltage_mvolt = 5000;

int32_t mot_voltg_adc = (motor_voltage_mvolt * 4096) / (3300 * 21);

int16_t motor_voltage = int16_t(mot_voltg_adc);

int16_t alpha = 0;
int16_t beta = 0;

SelectorMotorType motor_sel(motor_type, sup_vltg, alpha, beta, INT16_MIN);
SelectorInterconnectPwm4ch pwm_mux(pattern_pwm, motor_sel.getPwmChannels());
ModuleDriverPWM pwm(pwm_mode,
                    pwm_resolution,
                    sup_vltg,
                    pwm_mux.getPwmChannels());

void setup() {
  HAL_Init();
  // SystemClock_Config();
  MX_GPIO_Init();
  tim_pwm_init();

  SerialUSB.begin();    // Initialize SerialUSB
  while (!SerialUSB) {  // Wait for SerialUSB connection
    ;
  }
  SerialUSB.println("Ready!");

  pinMode(PB2, OUTPUT);
  pinMode(PA4, OUTPUT);

  digitalWrite(PB2, HIGH);
  digitalWrite(PA4, HIGH);

  analogReadResolution(12);

  pinMode(PA2, INPUT_ANALOG);
}

const float step = 0.15 * PI;  // Step of 0.05 PI
float angle = 0.0;

unsigned long previousMicros = 0;  // Store the last time the code was executed
const unsigned long interval = 500;  // Interval in microseconds

void loop() {
  sup_vltg = analogRead(PA2);
  motor_sel.tick();
  pwm_mux.tick();
  pwm.tick();

  setPwm(pwm.getPwmChannels());

  // Generate sin and cos values with amplitude 10000
  alpha = int16_t(sin(angle) * motor_voltage);
  beta = int16_t(cos(angle) * motor_voltage);

  // Increase the angle by step
  angle += step;

  // Reset the angle if it exceeds 2*PI
  if (angle >= 2 * PI) {
    angle -= 2 * PI;
  }

  // SerialUSB.print("Sup: ");
  // SerialUSB.print(sup_vltg);
  // SerialUSB.print("; PWM: ");
  // SerialUSB.print(pwm.getPwmChannels()[0]);
  //   SerialUSB.print("; ");
  // SerialUSB.println(pwm.getPwmChannels()[1]);

  // Add a delay to control the loop execution speed
  delayMicroseconds(100);  // 5 ms delay
}
