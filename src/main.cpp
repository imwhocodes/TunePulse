#include <Arduino.h>

#include "adc_setup.h"
#include "blocks_lib.h"
#include "dma_setup.h"
#include "gpio_setup.h"
#include "interrupt_setup.h"
#include "system_clock.h"
#include "timerPWM.h"

#include <USBDevice.h>
#include "blocks_lib.h"
#include "foc_setup.h"



#include <SPI.h>



int16_t pwm_resolution = 4000;  // not used yet

SPIClass SPI_1(PA7, PA6, PA5);
SPISettings ENC_SPI_SETTINGS(8000000, MSBFIRST, SPI_MODE1);


volatile uint16_t respond = 0;
void initEncoderCallback() {
  SPI_1.begin();
  SPI_1.beginTransaction(ENC_SPI_SETTINGS);
  pinMode(PC4, OUTPUT);
  digitalWrite(PC4, HIGH);
}

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

uint32_t readEncoderCallback() {
  digitalWriteFast(PC_4, LOW);         // start spi
  SPI_1.transfer16(0x8020);            // Send command word
  respond = SPI_1.transfer16(0x0000);  // Recieve position
  digitalWriteFast(PC_4, HIGH);        // end spi
  return respond << 17;                // normalize

}

void setup() {
    IO_Init();
    NVIC_Init();
    DMA_Init();
    ADC_Init();
    PWM_Init();

    digitalWriteFast(PB_2, HIGH);
    digitalWriteFast(PA_4, HIGH);
  
    initEncoderCallback();

  SerialUSB.begin();    // Initialize SerialUSB
  while (!SerialUSB) {  // Wait for SerialUSB connection
    ;
  }
  SerialUSB.println("Ready!");
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

// unsigned long previousMicros = 0;    // Store the last time the code was
// const unsigned long interval = 500;  // Interval in microseconds

// void loop() {
//   static int32_t count = 0;

//   // Print the round number
//   SerialUSB.println("Round: " + String(count));

//   // Get the start time
//   unsigned long startTime = micros();

//   for (int32_t i = 0; i < 1000000; i++) {
//     MOTOR_CONTROL::current_target_polar.ang += 1 << 24;
//     MOTOR_CONTROL::current_target_polar.rad = 250;
//     // MOTOR_CONTROL::angleRaw += 1 << 24;
//     MOTOR_CONTROL::angleRaw = readEncoderCallback();
//     // SerialUSB.print(">Angle: ");
//     // SerialUSB.println(
//     //     MOTOR_CONTROL::positionHandler.get_position_inst().split.angle);
//     // SerialUSB.print(">Rotations: ");
//     // SerialUSB.println(
//     //     MOTOR_CONTROL::positionHandler.get_position_inst().split.rotations);

//     // MOTOR_CONTROL::voltg_container.voltg_norm = adc_read_value(PA_2, 12) <<
//     // 3;  // TOOO SLOOOOW
//     MOTOR_CONTROL::voltg_container.voltg_norm = 600 << 3;
//     MOTOR_CONTROL::voltg_container.voltg_mv =
//         (MOTOR_CONTROL::voltg_container.voltg_norm * 69000) >> 15;  // Danger
//     MOTOR_CONTROL::tick();
//     setPwm(MOTOR_CONTROL::pwm.getPwmChannels());
//     delayMicroseconds(250);
//   }

//   // Get the end time
//   unsigned long endTime = micros();

//   // Calculate the elapsed time
//   unsigned long elapsedTime = endTime - startTime;

//   // Print the elapsed time
//   SerialUSB.println("Elapsed time: " + String(float(elapsedTime) / 1000000) +
//                     " microseconds");

//   count++;

//   // Delay to avoid flooding the serial output
//   delay(1000);
// }

