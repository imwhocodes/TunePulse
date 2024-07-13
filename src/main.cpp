#include <Arduino.h>
#include "..\lib\tools\macros\communication_channel_access_macros.h"

// Variable initialization
volatile int something = 0;     // Example variable
volatile float something1 = 0;  // Example variable

CHANNEL_WRITE_ACCESS(something, AccessKey::KEY_1, KEY_3);
CHANNEL_WRITE_ACCESS(something1, KEY_3);

// Main program
void setup() {
  // Initialize serial communication
  SerialUSB.begin(9600);

  // Set access levels

  // Attempt to read and write with different access levels
  int read_value = CHANNEL_READ(something);
  SerialUSB.println(read_value);  // Should print initial value

  CHANNEL_WRITE(something, 42, KEY_1);  // Should succeed
  read_value = CHANNEL_READ(something);
  SerialUSB.println(read_value);  // Should print 42

  // CHANNEL_WRITE(something, 84, LEVEL_2); // Should fail with compile-time
  // error
  CHANNEL_WRITE(something1, 126, GOD_MODE);  // Should succeed
  CHANNEL_WRITE(something1, 126, KEY_3);   // Should succeed

  CHANNEL_WRITE(something, 126, KEY_1);  // Should succeed

  CHANNEL_WRITE(something, 126, GOD_MODE);  // Should succeed
  read_value = CHANNEL_READ(something);
  SerialUSB.println(read_value);  // Should print 126
}

void loop() {
  // Empty loop
}
