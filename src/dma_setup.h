#ifndef DMA_SETUP_H
#define DMA_SETUP_H

#include <Arduino.h>
#include "config.h"

void MX_DMA_Init(void) {
    /* DMA controller clock enable */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMAMUX1);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

    /* DMA interrupt init */
    /* DMA1_Channel1_IRQn interrupt configuration */
    NVIC_SetPriority(DMA1_Channel1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), DMA_ADC_PRIORITY, 0));
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

#endif  // DMA_SETUP_H