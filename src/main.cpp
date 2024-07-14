#include <Arduino.h>
#include "..\lib\tools\macros\communication_channel_macros.h"

// Variable initialization


CHANNEL_INIT_DEFAULT(volatile int, something, 10);
CHANNEL_INIT_DEFAULT(volatile int, something1, 11);

CHANNEL_WRITE_ACCESS(something, AccessKey::KEY_1, KEY_3);
CHANNEL_WRITE_ACCESS(something1, KEY_3);

namespace fdf
{
  CHANNEL_INIT_DEFAULT(volatile int, myVar, 10);
} // namespace fdf

CHANNEL_INIT(volatile int, tryCombo, 10, KEY_1, KEY_2)



void setup() {
  // Initialize serial communication
  bool isDefault = CHANNEL_IS_DEFAULT(fdf::myVar);
  CHANNEL_SET_DEFAULT(fdf::myVar);

  CHANNEL_WRITE(tryCombo, 1, KEY_2);
  

  SerialUSB.begin();

  delay(2500);

  // Set access levels

  // Attempt to read and write with different access levels
  int read_value = CHANNEL_READ(something);
  SerialUSB.println(read_value);  // Should print initial value

  // Attempt to read and write with different access levels
  read_value = CHANNEL_IS_DEFAULT(something);
  SerialUSB.println(read_value);  // Should print initial value

  
  CHANNEL_WRITE(something, 42, KEY_1);  // Should succeed
  read_value = CHANNEL_READ(something);
  SerialUSB.println(read_value);  // Should print 42

    // Attempt to read and write with different access levels
  read_value = CHANNEL_IS_DEFAULT(something);
  SerialUSB.println(read_value);  // Should print initial value

  // CHANNEL_WRITE(something, 84, LEVEL_2); // Should fail with compile-time
  // error
  CHANNEL_WRITE(something1, 126, GOD_MODE);  // Should succeed
  CHANNEL_WRITE(something1, 126, KEY_3);   // Should succeed

  CHANNEL_WRITE(something, 126, KEY_1);  // Should succeed

  CHANNEL_WRITE(something, 126, GOD_MODE);  // Should succeed
  SerialUSB.println(CHANNEL_READ(something));  // Should print 126

  // Attempt to read and write with different access levels
  CHANNEL_SET_DEFAULT(something);
  SerialUSB.println(CHANNEL_READ(something));  // Should print initial value
}

void loop() {
  // Empty loop
}
