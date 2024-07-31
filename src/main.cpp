#include <Arduino.h>
#include "blocks_lib.h"
#include "adc_setup.h"
#include "dma_setup.h"
#include "gpio_setup.h"
#include "nvic.h"
#include "system_clock.h"
#include "timerPWM.h"

// Variables for motor and PWM configuration
MotorType motor_type = STEPPER;
PatternPWM pattern_pwm = ABCD;
ModePWM pwm_mode = ALLIGNED_GND;

int16_t sup_vltg = 10000;       // not used yet
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

uint16_t current_sensor_A, current_sensor_B, voltage_vref, temperature;

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
  if (htim->Instance == TIM2) {
    static bool underflow = true;
    if (underflow = !underflow) {
      // TODO use TIM2_Set_PWM_Values here
      TIM2_Set_PWM_Values(pwm.getPwmChannels());

      // NVIC_SetPendingIRQ(EXTI0_IRQn);
      // NVIC_SetPendingIRQ(EXTI1_IRQn);
      current_sensor_A = dma_buffer[0];
      current_sensor_B = dma_buffer[1];
      sup_vltg = dma_buffer[2] << 3;
      voltage_vref = dma_buffer[3] << 3;
      temperature = dma_buffer[4];
    } else {
      ADC1_StartDMAConversion();
    }
  }
}

void init_debug_pin() {
  // EXP_TX | pc10
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

extern "C" void EXTI0_IRQHandler(void) {
  NVIC_ClearPendingIRQ(EXTI0_IRQn);

  // TODO Add high speed loop math implementation here
}

extern "C" void EXTI1_IRQHandler(void) {
  NVIC_ClearPendingIRQ(EXTI1_IRQn);

  // TODO Add FOC implementation here
  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10);
  delayMicroseconds(1);
  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10);
}

void setup() {
  init_debug_pin();
  GPIO_Init();
  // NVIC_Init();
  MX_DMA_Init();
  MX_ADC1_Init((uint16_t*)dma_buffer, DMA_ADC_BUFFER_SIZE);
  MX_ADC1_Start();
  MX_TIM2_Init();
  MX_TIM2_Start();


  digitalWriteFast(PB_2, HIGH);
  digitalWriteFast(PA_4, HIGH);
}

const float step = 0.01 * PI;  // Step of 0.05 PI
float angle = 0.0;

unsigned long previousMicros = 0;  // Store the last time the code was executed
const unsigned long interval = 500;  // Interval in microseconds

void loop() {
  motor_sel.tick();
  pwm_mux.tick();
  pwm.tick();

  // Generate sin and cos values with amplitude 10000
  alpha = int16_t(sin(angle) * motor_voltage);
  beta = int16_t(cos(angle) * motor_voltage);

  // Increase the angle by step
  angle += step;

  // Reset the angle if it exceeds 2*PI
  if (angle >= 2 * PI) {
    angle -= 2 * PI;
  }

  // SerialUSB.print(">Sup: " + String(sup_vltg));
  // SerialUSB.print(">Temp: " + String(temperature));
  // SerialUSB.print(">Vref: " + String(voltage_vref));

  // Add a delay to control the loop execution speed
  delayMicroseconds(3000);
}
