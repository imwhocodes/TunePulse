#ifndef NVIC_H
#define NVIC_H

#include <Arduino.h>
#include "config.h"

#include "adc_setup.h"
#include "blocks_lib.h"
#include "dma_setup.h"
#include "gpio_setup.h"
#include "interrupt_setup.h"
#include "system_clock.h"
#include "timerPWM.h"

void NVIC_Init() {
    /* DMA interrupt init */
    /* DMA1_Channel1_IRQn interrupt configuration */
    NVIC_SetPriority(DMA1_Channel1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), DMA_ADC_PRIORITY, 0));
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);

    /* PWM TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM2_IRQn, TIM2_INT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

int16_t sup_vltg = 10000;  // not used yet
int16_t current_sensor_A, current_sensor_B, voltage_vref, temperature;
extern "C" void DMA1_Channel1_IRQHandler(void) {
    // Check whether DMA transfer complete caused the DMA interruption
    if (LL_DMA_IsActiveFlag_TC1(DMA1) == 1) {
        LL_DMA_ClearFlag_TC1(DMA1);  // Clear flag DMA transfer complete

        // // Complete DMA transfer handler
        // HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10);
        ADC_get_values(current_sensor_A, current_sensor_B, sup_vltg, voltage_vref, temperature);
    }

    // Check whether DMA half transfer caused the DMA interruption
    if (LL_DMA_IsActiveFlag_HT1(DMA1) == 1) {
        LL_DMA_ClearFlag_HT1(DMA1);  // Clear flag DMA half transfer

        // // Half DMA transfer handler
        // HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10);
    }
}

#endif  // NVIC_H