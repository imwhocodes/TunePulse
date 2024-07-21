#include <Arduino.h>
#include "blocks_lib.h"
#include "stm32g4xx_hal.h"

// Variables for motor and PWM configuration
MotorType motor_type = STEPPER;
PatternPWM pattern_pwm = ABCD;
ModePWM pwm_mode = ALLIGNED_GND;

int16_t sup_vltg = 10000;           // not used yet
int16_t pwm_resolution = 6000 * 2;  // not used yet
int16_t motor_voltage = 100;

int16_t alpha = 0;
int16_t beta = 0;

SelectorMotorType motor_sel(motor_type, sup_vltg, alpha, beta, INT16_MIN);
SelectorInterconnectPwm4ch pwm_mux(pattern_pwm, motor_sel.getPwmChannels());
ModuleDriverPWM pwm(pwm_mode,
                    pwm_resolution,
                    sup_vltg,
                    pwm_mux.getPwmChannels());



TIM_HandleTypeDef htim2;

void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if (timHandle->Instance == TIM2) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }
}

void MX_TIM2_Init(void) {
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  htim2.Init.Period = 4000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
    Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
    Error_Handler();
  }

  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) {
    Error_Handler();
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK) {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim2);
}

void setPwm(const int16_t* pwmValues) {
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwmValues[0]);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwmValues[1]);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, pwmValues[2]);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, pwmValues[3]);
}

void setup() {
  HAL_Init();
  // SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM2_Init();

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

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


const float step = 0.025 * PI;  // Step of 0.05 PI
float angle = 0.0;


unsigned long previousMicros = 0;  // Store the last time the code was executed
const unsigned long interval = 500;  // Interval in microseconds


void loop() {

  sup_vltg = analogRead(PA2);
  motor_sel.tick();
  pwm_mux.tick();
  pwm.tick();

  SerialUSB.print("sup_vltg: ");
  SerialUSB.print(sup_vltg);
  SerialUSB.print("; alpha: ");
  SerialUSB.print(alpha);
  SerialUSB.print("; beta:");
  SerialUSB.println(beta);

  setPwm(pwm.getPwmChannels());


  // Generate sin and cos values with amplitude 10000
  alpha = int16_t(int32_t((int32_t(sin(angle) * 100) * motor_voltage) / sup_vltg));
  beta = int16_t(int32_t((int32_t(cos(angle) * 100) * motor_voltage) / sup_vltg));

  // Increase the angle by step
  angle += step;

  // Reset the angle if it exceeds 2*PI
  if (angle >= 2 * PI) {
    angle -= 2 * PI;
  }

  // Output PWM channel values to SerialUSB
  // const int32_t* channels_pwm = pwm.getPwmChannels();
  // SerialUSB.print("PWM Channels: ");
  // for (int i = 0; i < 4; ++i) {
  //   SerialUSB.print(channels_pwm[i]);
  //   if (i < 3) {
  //     SerialUSB.print(", ");
  //   }
  // }
  // SerialUSB.println();
// const int32_t* channels = pwm_mux.getPwmChannels();
//     SerialUSB.print("MUX Channels: ");
//   for (int i = 0; i < 4; ++i) {
//     SerialUSB.print(channels[i]);
//     if (i < 3) {
//       SerialUSB.print(", ");
//     }
//   }
//   SerialUSB.println();

  // Add a delay to control the loop execution speed
  delayMicroseconds(50);  // 5 ms delay
}
