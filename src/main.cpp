#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include <SimpleFOC.h>
#include <SimpleFOCDrivers.h>

// This includes the target selected by the PIO build environment
// The include error can be dismissed until we find a fix for the dev env.
#include "bootloaderTools.h"
#include "target.h"

#ifdef TARGET_CLN17_V1_5
#warning "Target: CLN17 v1.5 not yet implemented"
#endif

#ifdef TARGET_CLN17_V2_0
#warning "Target: CLN17 v2 not yet implemented"
#endif

void setup() {
  SerialUSB.begin(115200);
  pinMode(PINOUT::LED_GRN, OUTPUT);
  pinMode(PINOUT::LED_RED, OUTPUT);
  pinMode(PINOUT::LED_BLU, OUTPUT);

  // Initialize LEDs
  digitalWrite(PINOUT::LED_GRN, HIGH);
  digitalWrite(PINOUT::LED_RED, HIGH);
  digitalWrite(PINOUT::LED_BLU, HIGH);
}

void loop() {
  // Jump to bootloader if 'b' is received
  if (SerialUSB.available() > 0) {
    char receivedChar = SerialUSB.read();
    if (receivedChar == 'b') {
      JumpToBootloader();
    }
  }

  SerialUSB.println("Hello World!");
  digitalToggle(PINOUT::LED_GRN);
  delay(100);
}
