#include <Arduino.h>
#include "blocks_lib.h"

MotorType motor_type = STEPPER;
PatternPWM pattern_pwm = ABCD;

int32_t sup_vltg = 200; // not done yet

int32_t alpha = 0;
int32_t beta = 0;

SelectorMotorType motor_sel(motor_type, sup_vltg, alpha, beta);
SelectorInterconnectPwm4ch pwm_mux(pattern_pwm, motor_sel.getPwmChannels());

const float step = 0.05 * PI;  // шаг в 0.05 PI
float angle = 0.0;

void setup() {
  SerialUSB.begin();    // Инициализация SerialUSB
  while (!SerialUSB) {  // Ждем подключения SerialUSB
    ;
  }
  
  // Настройка частоты ШИМ на 30кГц для TIM2
  analogWriteFrequency(30000);  // Устанавливаем частоту ШИМ на 30кГц

  pinMode(PB2, OUTPUT);
  pinMode(PA4, OUTPUT);

    digitalWrite(PB2, HIGH);
  digitalWrite(PA4, HIGH);

  analogWriteResolution(12);



}

void loop() {
  motor_sel.tick();
  pwm_mux.tick();

  // Генерация значений sin и cos с амплитудой 10000
  alpha = int32_t(100 * sin(angle));
  beta = int32_t(100 * cos(angle));

  // Увеличиваем угол на шаг
  angle += step;

  // Если угол больше 2*PI, сбрасываем его
  if (angle >= 2 * PI) {
    angle -= 2 * PI;
  }

  // Вывод значений getPwmChannels() в SerialUSB
  const int32_t* channels = pwm_mux.getPwmChannels();
//   SerialUSB.print("PWM Channels: ");
//   for (int i = 0; i < 4; ++i) {
//     SerialUSB.print(channels[i]);
//     if (i < 3) {
//       SerialUSB.print(", ");
//     }
//   }
//   SerialUSB.println();

//   Настройка ШИМ сигналов на выводах PA0, PA1, PB10, PB11
  analogWrite(PA0, channels[2]);
  analogWrite(PA1, channels[3]);
  analogWrite(PB11, channels[1]);
  analogWrite(PB10, channels[0]);

  // Добавляем задержку для контроля скорости выполнения цикла
  delayMicroseconds(5000);  // задержка 100 мс
}