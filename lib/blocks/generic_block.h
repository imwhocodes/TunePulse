/**
 * @file generic_modules.h
 * @brief Header file for the SIGMA architecture controller layer modules.
 *
 * This file contains the definitions for the generic Block
 * which form the core of the controller layer in the SIGMA architecture.
 */

#ifndef GENERIC_BLOCK_H
#define GENERIC_BLOCK_H

#include <float.h>
#include <inttypes.h>
#include "control_modes.h"

struct VectorAxes2D_I32 {
  int32_t sin;  // SIN or Projection on X / sin / D axis
  int32_t cos;  // COS or Projection on Y / B / Q axis
} __attribute__((packed));

struct VectorPolar2D_I32 {
  int32_t ang;  // Angle Int1.31
  int32_t rad;  // Radius
} __attribute__((packed));

struct VectorAxes2D_I16 {
  int16_t sin;  // Projection on X / sin / D axis
  int16_t cos;  // Projection on Y / B / Q axis
} __attribute__((packed));

// ################### GENERIC BLOCK #################################

/**
 * @brief Base class for all elements of the Controller layer in SIGMA
 * architecture.
 *
 * The Block class serves as the base class for all math functions.
 * It defines a pure virtual function tick(), which is the only block callback
 * and must be implemented by derived classes.
 */

class Block {
 public:
  /**
   * @brief Pure virtual function of Block callback.
   */
  virtual void tick() = 0;
};

// ################### MACROS #################################

/**
 * @brief Macro to define an input channel in a Block.
 * This macro creates a protected const reference to an input variable.
 * @param type The data type of the input.
 * @param name The name of the input variable.
 */
#define BLOCK_INPUT(type, name) \
 protected:                     \
  const type& name##_;

/**
 * @brief Macro to define an output channel in a Block.
 * This macro creates a protected reference to an output variable.
 * @param type The data type of the output.
 * @param name The name of the output variable.
 */
#define BLOCK_OUTPUT(type, name)   \
 protected:                        \
  type name##_;                    \
                                   \
 public:                           \
  const type& get_##name() const { \
    return name##_;                \
  }

#endif  // GENERIC_BLOCK_H