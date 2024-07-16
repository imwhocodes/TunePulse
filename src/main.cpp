#include <Arduino.h>
#include "..\lib\system_layout\communication_channels.h"
#include "..\lib\tools\macros\communication_channel_macros.h"

// Variable initialization
#include "..\blocks\regulator_pid_feedforward.h"
#include "SPI.h"

struct MyClassParams {
  int param1;
  float param2;
  bool param3;

  // Constructor to enforce initialization
  MyClassParams(int p1, float p2, bool p3)
      : param1(p1), param2(p2), param3(p3) {}
};

// Usage example
int32_t in_actual = 0;
int32_t in_reference = 1000;
int32_t in_feedforward = 500;
int32_t in_Kp = 1000;
int32_t in_Ki = 500;
int32_t in_Kd = 100;
int32_t in_Kff = 200;
int32_t in_output_limit = 10000;
int32_t out_value;
int32_t out_error;

INIT_REGULATOR_PID_FF(myPIDController,
                      in_reference,
                      in_feedforward,
                      in_actual,
                      in_Kp,
                      in_Ki,
                      in_Kd,
                      in_Kff,
                      in_output_limit);

void setup() {
  SerialUSB.begin(9600);
  while (!SerialUSB) {
    ;  // wait for SerialUSB port to connect
  }
  myPIDController.tick();
  SerialUSB.println("PID Controller Initialized");
}

void loop() {
  delay(1000);  // Delay for 1 second
}