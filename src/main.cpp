#include <Arduino.h>
#include "block_absolute_position.h"

uint32_t angle_raw = 0;

AbsPosition pos_offst(0, 0);
int32_t frequency = 1;
// Instantiate the BlockAbsolutePosition object
BlockAbsolutePosition block(angle_raw, pos_offst, frequency);

void setup() {
  // Initialize SerialUSB at a baud rate of 115200
  SerialUSB.begin(115200);
  while (!SerialUSB) {
    // Wait for SerialUSB to initialize
  }
}

int32_t test = 0;

void loop() {
  // Simulate changing the angle_raw input

  test += 1 << 24 ;  // Increment angle for testing
  angle_raw = test;

  // Call the tick method
  block.tick();

  // Read and print the position and speed 
  int32_t angle = block.get_position_inst().split.angle;
  int32_t rot = block.get_position_inst().split.rotations;
  int64_t speed = block.get_speed_inst();

  // Print the values to SerialUSB
  SerialUSB.print("Raw angle: ");
  SerialUSB.print(angle_raw);
  SerialUSB.print("; Angle: ");
  SerialUSB.print(angle);
  SerialUSB.print("; Rot: ");
  SerialUSB.print(rot);
  SerialUSB.print("; Speed: ");
  SerialUSB.println(speed);
  SerialUSB.println();

  // Delay for a bit before the next loop iteration
  delay(100);
}

// #include <Arduino.h>
// #include "blocks_lib.h"
// #include "timerPWM.h"

// // Variables for motor and PWM configuration
// MotorType motor_type = STEPPER;
// PatternPWM pattern_pwm = ABCD;
// ModePWM pwm_mode = ALLIGNED_GND;

// int16_t sup_vltg = 10000;           // not used yet
// int16_t pwm_resolution = 4000;  // not used yet

// int32_t motor_voltage_mvolt = 5000;

// int32_t mot_voltg_adc = (motor_voltage_mvolt * 4096) / (3300 * 21);

// int16_t motor_voltage = int16_t(mot_voltg_adc);

// int16_t alpha = 0;
// int16_t beta = 0;

// SelectorMotorType motor_sel(motor_type, sup_vltg, alpha, beta, INT16_MIN);
// SelectorInterconnectPwm4ch pwm_mux(pattern_pwm, motor_sel.getPwmChannels());
// ModuleDriverPWM pwm(pwm_mode,
//                     pwm_resolution,
//                     sup_vltg,
//                     pwm_mux.getPwmChannels());

// void setup() {
//   HAL_Init();
//   // SystemClock_Config();
//   MX_GPIO_Init();
//   tim_pwm_init();

//   SerialUSB.begin();    // Initialize SerialUSB
//   while (!SerialUSB) {  // Wait for SerialUSB connection
//     ;
//   }
//   SerialUSB.println("Ready!");

//   pinMode(PB2, OUTPUT);
//   pinMode(PA4, OUTPUT);

//   digitalWrite(PB2, HIGH);
//   digitalWrite(PA4, HIGH);

//   analogReadResolution(12);

//   pinMode(PA2, INPUT_ANALOG);
// }

// const float step = 0.15 * PI;  // Step of 0.05 PI
// float angle = 0.0;

// unsigned long previousMicros = 0;  // Store the last time the code was
// executed const unsigned long interval = 500;  // Interval in microseconds

// void loop() {
//   sup_vltg = analogRead(PA2);
//   motor_sel.tick();
//   pwm_mux.tick();
//   pwm.tick();

//   setPwm(pwm.getPwmChannels());

//   // Generate sin and cos values with amplitude 10000
//   alpha = int16_t(sin(angle) * motor_voltage);
//   beta = int16_t(cos(angle) * motor_voltage);

//   // Increase the angle by step
//   angle += step;

//   // Reset the angle if it exceeds 2*PI
//   if (angle >= 2 * PI) {
//     angle -= 2 * PI;
//   }

//   // SerialUSB.print("Sup: ");
//   // SerialUSB.print(sup_vltg);
//   // SerialUSB.print("; PWM: ");
//   // SerialUSB.print(pwm.getPwmChannels()[0]);
//   //   SerialUSB.print("; ");
//   // SerialUSB.println(pwm.getPwmChannels()[1]);

//   // Add a delay to control the loop execution speed
//   delayMicroseconds(100);  // 5 ms delay
// }
