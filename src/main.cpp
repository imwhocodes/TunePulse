#include <Arduino.h>

#include "ControlModes/PWMFOC.h"
#include "ControlModes/VoltageCurrentMode.h"
#include "Module.h"

SelectorEstimatorModes mode = SelectorEstimatorModes::voltageEstimationMode;
int32_t crnt_d = 0, crnt_q = 0;
int32_t vltg_d = 0, vltg_q = 0;

void timer_callback_mock_1() {
  // DISABLE INTERRUPTS
  const SelectorEstimatorModes i_mode = mode;
  const int32_t i_crnt_d = crnt_d;
  const int32_t i_crnt_q = crnt_q;
  // ENABLE INTERRUPTS

  MODULE(sest, SelectorEstimator(i_mode, i_crnt_q, i_crnt_d))

  // DISABLE INTERRUPTS
  vltg_d = sest.VOLTG_D;
  vltg_q = sest.VOLTG_Q;
  // ENABLE INTERRUPTS
}

void timer_callback_mock_2() {
  // DISABLE INTERRUPTS
  const SelectorEstimatorModes i_mode = mode;
  const int32_t i_crnt_d = crnt_d;
  const int32_t i_crnt_q = crnt_q;
  // ENABLE INTERRUPTS

  MODULE(pwmfoc, PWMFOC_ControlMode(i_mode, i_crnt_q, i_crnt_d))

  // DISABLE INTERRUPTS
  vltg_d = pwmfoc.VOLTG_D_ADJ;
  vltg_q = pwmfoc.VOLTG_Q_ADJ;
  // ENABLE INTERRUPTS
}

void setup() {
  SerialUSB.begin();

  SerialUSB.println(String(mode) + " " + String(vltg_d) + " " + String(vltg_q));
  delay(200);

  timer_callback_mock_2();

  SerialUSB.println(String(mode) + " " + String(vltg_d) + " " + String(vltg_q));
  delay(200);

  mode = SelectorEstimatorModes::currentFeedbackMode;
  timer_callback_mock_2();

  SerialUSB.println(String(mode) + " " + String(vltg_d) + " " + String(vltg_q));
  delay(200);

  delay(1000);
}

void loop() {
  // Empty loop
}
