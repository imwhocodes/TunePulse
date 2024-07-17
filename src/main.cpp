#include <Arduino.h>
#include "..\lib\tools\macros\communication_channel_macros.h"

// Variable initialization
#include "..\blocks\blocks_lib.h"

// Global variables for the integrator input and output
float input_sin = 0.0f, input_cos = 1.0f, rotation_sin = 0.707f, rotation_cos = 0.707f;

// Create instances of the classes
MathRotateVectorSinCos rotate1(input_sin, input_cos, rotation_sin, rotation_cos);
MathRotateVectorSinCos rotate2(rotate1.output_sin, rotate1.output_cos, rotation_sin, rotation_cos);


void setup() {
  // Initialize serial communication
  SerialUSB.begin();
  while (!SerialUSB);

  SerialUSB.println("Rotation Example");
}

void loop() {
  // Perform rotation 45deg
  rotate1.tick();
  // Perform another rotation 45deg
  rotate2.tick();

      // Print results
  SerialUSB.print("Sin1: ");
  SerialUSB.println(rotate1.output_sin);
  SerialUSB.print("Cos1: ");
  SerialUSB.println(rotate1.output_cos);

  // Print results
  SerialUSB.print("Sin2: ");
  SerialUSB.println(rotate2.output_sin);
  SerialUSB.print("Cos2: ");
  SerialUSB.println(rotate2.output_cos);

  input_sin = rotate2.output_sin;
  input_cos = rotate2.output_cos;

  // Simulate a delay for demonstration purposes
  delay(1000);
}