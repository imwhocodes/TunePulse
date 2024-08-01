#include <Arduino.h>
#include "adc_setup.h"
#include "blocks_lib.h"
#include "dma_setup.h"
#include "gpio_setup.h"
#include "interrupt_setup.h"
#include "system_clock.h"
#include "timerPWM.h"

// Variables for motor and PWM configuration
MotorType motor_type = STEPPER;
PatternPWM pattern_pwm = ABCD;
ModePWM pwm_mode = ALLIGNED_GND;


int16_t pwm_resolution = 4000;  // not used yet

int32_t motor_voltage_mvolt = 5000;

int32_t mot_voltg_adc = (motor_voltage_mvolt * 4096) / (3300 * 21);

int16_t motor_voltage = int16_t(mot_voltg_adc);

int16_t alpha = 0;
int16_t beta = 0;

SelectorMotorType motor_sel(motor_type, sup_vltg, alpha, beta, INT16_MIN);
SelectorInterconnectPwm4ch pwm_mux(pattern_pwm, motor_sel.getPwmChannels());
ModuleDriverPWM pwm(pwm_mode, pwm_resolution, sup_vltg, pwm_mux.getPwmChannels());

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    if (htim->Instance == TIM2) {
        static bool underflow = true;
        if (underflow = !underflow) {
            TIM2_Set_PWM_Values(pwm.getPwmChannels());
        } else {
            ADC1_StartDMAConversion();
        }
    }
}

void setup() {
    IO_Init();
    NVIC_Init();
    DMA_Init();
    ADC_Init();
    PWM_Init();

    digitalWriteFast(PB_2, HIGH);
    digitalWriteFast(PA_4, HIGH);
}

const float step = 0.01 * PI;  // Step of 0.05 PI
float angle = 0.0;

unsigned long previousMicros = 0;    // Store the last time the code was executed
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


    delayMicroseconds(3000);
}
