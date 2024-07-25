/**
 * @file data_types.h
 * @brief Header file defining data types that used in TUNEPULSE
 */

#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <inttypes.h>

/**
 * @typedef VoltgChannelslNormlzd
 * @brief An array to store normalized phase voltages.
 * This type defines an array of four 16-bit integers representing the normalized phase
 * voltages.
 */
using VoltgChannelslNormlzd = int16_t[4];

/**
 * @struct VectorAxes2D_I32
 * @brief A structure to store 2D vector components with 32-bit integers.
 * Represents a 2D vector using 32-bit integers for its components. It contains the sine
 * (X / sin / D axis) and cosine (Y / B / Q axis) projections.
 */
struct VectorAxes2D_I32 {
  int32_t sin; /* SIN or Projection on X / sin / D axis */
  int32_t cos; /* COS or Projection on Y / B / Q axis */
} __attribute__((packed));

/**
 * @struct VectorPolar2D_I32
 * @brief A structure to store polar coordinates with 32-bit integers.
 * Represents a vector in polar coordinates using 32-bit integers for the angle and
 * radius.
 */
struct VectorPolar2D_I32 {
  int32_t ang; /* Angle in Int1.31 format */
  int32_t rad; /* Radius */
} __attribute__((packed));

/**
 * @struct VectorAxes2D_I16
 * @brief A structure to store 2D vector components with 16-bit integers.
 * Represents a 2D vector using 16-bit integers for its components. It contains the sine
 * (X / sin / D axis) and cosine (Y / B / Q axis) projections.
 */
struct VectorAxes2D_I16 {
  int16_t sin; /* Projection on X / sin / D axis */
  int16_t cos; /* Projection on Y / B / Q axis */
} __attribute__((packed));

#endif  // DATA_TYPES_H