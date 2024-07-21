#include <Arduino.h>
#include "blocks_lib.h"
#include "bootloaderTools.h"
#include "target.h"

// Initial channel values for each selector instance
int32_t channels0[4] = {1000, 2000, 3000, 4000};

PatternPWM mode = ABCD;  // Select the mode

// Create 10 instances of SelectorInterconnectPwm4ch
static SelectorInterconnectPwm4ch selector0(mode, channels0);

uint32_t tickCount = 0;   // Counter for the number of ticks
int32_t outputValues[4];  // Array to store output values

void setup() {
  SerialUSB.begin();
  delay(1000);

  // Initialize raw pins
  pinMode(PINOUT::SYS_SW1, INPUT);
  pinMode(PINOUT::LED_GRN, OUTPUT);
  pinMode(PINOUT::LED_RED, OUTPUT);
  pinMode(PINOUT::LED_BLU, OUTPUT);

  // Enable DWT and the cycle counter
  if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  }

  // Bootloader check
  // Jump to bootloader if button is pressed during setup
  if (digitalRead(PINOUT::SYS_SW1) == LOW) {
    digitalWrite(PINOUT::LED_GRN, HIGH);  // green led = bootloader entry
    JumpToBootloader();
  }
}

void loop() {
  // Start cycle count
  uint32_t startTickTime = DWT->CYCCNT;

  // Update the channel pointers for all selector instances based on the mode
  selector0.tick();

  // Example: Print the output values of selector0
  outputValues[0] = selector0.getPwmChannels()[0];
  outputValues[1] = selector0.getPwmChannels()[1];
  outputValues[2] = selector0.getPwmChannels()[2];
  outputValues[3] = selector0.getPwmChannels()[3];
  // End cycle count
  uint32_t endTickTime = DWT->CYCCNT;

  // Calculate the time taken for the tick and array access in clock cycles
  uint32_t tickDuration = endTickTime - startTickTime;

  // Print the tick duration in clock cycles
  SerialUSB.print("Tick duration: ");
  SerialUSB.print(tickDuration);
  SerialUSB.println(" clock cycles");

  // Print the new array
  for (int i = 0; i < 4; ++i) {
    SerialUSB.print(outputValues[i]);
    SerialUSB.print(" ");
  }
  SerialUSB.println();

  tickCount++;

  // Change mode every 5 cycles
  if (tickCount % 2 == 0) {
    constexpr PatternPWM patterns[] = {PatternPWM::ABCD, PatternPWM::ACDB,
                                       PatternPWM::ADBC, PatternPWM::DCAB};
    mode = patterns[(tickCount / 2) & 4];
    SerialUSB.print("Mode changed to: ");
    SerialUSB.println(mode);
    delay(1000);
  }

  // Increment channel values for all selector instances
  for (int i = 0; i < 4; ++i) {
    channels0[i]++;
  }

  delay(100);  // Wait for 100 milliseconds before the next iteration
}
