#ifndef MATH_STATIC_H
#define MATH_STATIC_H

#include <Arduino.h>
#include <inttypes.h>
#include "blocks_lib.h"

static VectorAxes2D_I16 getSinCos(int32_t angle) {
  float radians = (float)(angle)*PI / INT32_MAX;

  // Вычисление синуса и косинуса
  float sinValue = sin(radians) * INT16_MAX;
  float cosValue = cos(radians) * INT16_MAX;

  return {.sin = int16_t(sinValue), .cos = int16_t(cosValue)};
}

static VectorAxes2D_I16 rotateSinCosVector(const VectorAxes2D_I16& vector,
                                           const VectorAxes2D_I16& rotation) {
  // Compute the resultant sine and cosine value after rotation
  int32_t out_sin_raw = vector.sin * rotation.cos + vector.cos * rotation.sin;
  int32_t out_cos_raw = vector.cos * rotation.cos - vector.sin * rotation.sin;
  return {.sin = int16_t(out_sin_raw), .cos = int16_t(out_cos_raw)};
}

static int16_t voltageNormilize(int32_t voltg_mv, int32_t voltg_sense_max_mv) {
  int64_t voltage_norm_raw =
      (static_cast<int64_t>(voltg_mv) << 15) / voltg_sense_max_mv;
  return int16_t(voltage_norm_raw);
}

#endif  // MATH_STATIC_H