/**
 * @file voltage_estimation.h
 * @brief Header file to manage voltage estimation across the motor
 *
 */

#ifndef BLOCK_VOLTAGE_ESTIMATION_H
#define BLOCK_VOLTAGE_ESTIMATION_H

#include "generic_block.h"

class Voltage_Estimation : public Block {
  BLOCK_INPUT(int32_t, currentSinRef);    // Measured current sin reference
  BLOCK_INPUT(int32_t, currentCosRef);    // Measured current cos reference
  BLOCK_INPUT(int32_t, motorResistance);  // Resistance of motor in ohms
  BLOCK_INPUT(int32_t, voltageSupply);    // Supply voltage in volts
  BLOCK_OUTPUT(int32_t, voltageSin);  // Output calculated voltage sin reference
  BLOCK_OUTPUT(int32_t, voltageCos);  // Output calculated voltage cos reference

 public:
  /**
   *
   */
  Voltage_Estimation(const int32_t& currentSinRef,
                     const int32_t& currentCosRef,
                     const int32_t& motorResistance,
                     const int32_t& voltageSupply,
                     const int32_t& voltageSin,
                     const int32_t& voltageCos)
      : currentSinRef_(currentSinRef),
        currentCosRef_(currentCosRef),
        motorResistance_(motorResistance),
        voltageSupply_(voltageSupply),
        voltageSin_(voltageSin),
        voltageCos_(voltageCos) {}
};

#endif  // BLOCK_VOLTAGE_ESTIMATION_H