#include <Arduino.h>

namespace TP_LL_DATA
{

/**
 * PWM if voltage > 0;
 * Braking if voltage = 0;
 * Brake PWM if voltage < 0;
 * Disable if voltage = INT32_MIN;
 */
volatile int32_t V_CH1 = INT32_MIN, V_CH2 = INT32_MIN, V_CH3 = INT32_MIN, VCH4 = INT32_MIN;

/**
 * Fixed point encoder position value
 */
volatile union {
    int64_t POSITION;
    struct {
        int32_t ROTATION;
        int32_t ANGLE;
    };
} POSITION;


} // namespace TP_LL_DATA
