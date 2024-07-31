#ifndef NVIC_H
#define NVIC_H

#include <Arduino.h>
#include "config.h"

void NVIC_Init() {
    // Interrups priority grouping field is NVIC_PRIORITYGROUP_4
    // This means 4 bits for pre-emption priority, 0 bit for subpriority

    // Enable and set priority for EXTI0 interrupt
    HAL_NVIC_SetPriority(EXTI0_IRQn, EXTI0_INT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    // Enable and set priority for EXTI1 interrupt
    HAL_NVIC_SetPriority(EXTI1_IRQn, EXTI1_INT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}

#endif  // NVIC_H