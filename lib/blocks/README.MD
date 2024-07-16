# Template for Blocks

## Base Class `Block`

The `Block` class serves as the base class for all blocks in the Controller layer. It defines a single pure virtual function `tick()` that derived classes must implement.

```с++
/**
 * @brief Base interface for all elements of the Controller layer in SIGMA architecture.
 *
 * The Block class serves as the base class for all controller elements.
 * It defines a pure virtual function tick(), which is the main module callback
 * and must be implemented by derived classes.
 */
class Block {
 public:
  /**
   * @brief Pure virtual function of Block callback.
   */
  virtual void tick() = 0;
};
```

## Derivatives

Any class derived from the `Block` class must implement the `Constructor` and the single public method `tick()`.

During initialization, references to the Data Channels that the function will work with are set, along with the initial or default values of internal variables (if necessary).

Then, after initialization, all calculations will be performed when the `tick()` method of the corresponding class instance is called. This method also will write data to the Data Channels specified during initialization.

The `tick()` method must be overridden with the specific calculations characteristic of the class's functionality and specified output

### Macros for Input and Output Channels

Use macros to define input and output channels to ensure the correct type of variable and improve readability.

```С++
#define BLOCK_INPUT(type, name) \
 protected:                     \
  const type& name##_;

#define BLOCK_OUTPUT(type, name) \
 protected:                      \
  type name##_;                  \
                                 \
 public:                         \
  const type& name = name##_;
```

### Example of Usage

The `Integrator` class integrates the input value over time and stores the result in the output variable.

```С++
#ifndef BLOCK_INTEGRATOR_H
#define BLOCK_INTEGRATOR_H

#include "generic_block.h"
/**
 * @brief Example implementation of an Integrator inheriting from Block.
 * This class integrates the input value over time, storing the result in the
 * output variable.
 */
class Integrator : public Block {
  // Define input and output channels
  BLOCK_INPUT(double, input_value)
  BLOCK_OUTPUT(double, integrated_value)

 private:
  // Internal state variable to keep track of the integral
  double integral_;

 public:
  /**
   * @brief Integrator constructor with linking input and output.
   * @param input The input value to be integrated.
   * @param output The variable where the integrated value will be stored.
   */
  Integrator(const double& input)
      : input_value_(input), integral_(0.0) {}

  /**
   * @brief Main callback function that performs the integration.
   * This function reads the input, updates the integral, and sets the output.
   */
  void tick() override {
    // Update the integral with the current input value
    integrated_value_ += input_value_;
  }
};

#endif // BLOCK_INTEGRATOR_H
```

#### Explanation

- **Constructor**: Initializes the input and output references and an internal state variable `integral_`.
- **tick() Function**: Reads the input (`input_value`), updates the integral (`integral_`), and sets the output (`integrated_value`).

#### Usage Example

Here is an example of how to use the `Integrator` class:

```С++
#include <Arduino.h>
#include "integrator_block.h" // Path to the library

// Global variables for the integrator input and output
double input_value = 1.0, integrated_value = 0.0;

// Create an instance of the Integrator class
Integrator integrator(input_value, integrated_value);

void setup() {
  // Initialize the USB serial communication
  SerialUSB.begin();

  // Wait for the serial communication to establish
  while (!SerialUSB);

  // Print initial values
  SerialUSB.println("Integrator Example");
}

void loop() {
  // Call the tick function to update the integrated value
  integrator.tick();
  SerialUSB.println("Integrated Value: " + String(integrated_value));
  
  // Simulate input value increment for demonstration
  input_value += 0.1;
  delay(100);
}
```
