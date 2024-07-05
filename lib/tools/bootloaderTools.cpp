
#include "bootloaderTools.h"

void JumpToBootloader(void) {
  /*
  In addition to the patterns described above, the user can execute bootloader
  by performing a jump to system memory from user code. Before jumping to
  bootloader: • Disable all peripheral clocks • Disable used PLL • Disable
  interrupts • Clear pending interrupts
  */

  //! As we improve the code, we will add more steps to ensure a safe jump to
  //! the bootloader.

  /* Disable all interrupts */
  __disable_irq();

  /* Disable Systick timer */
  SysTick->CTRL = 0;
  SysTick->LOAD = 0;
  SysTick->VAL = 0;

  /* Set the clock to the default state */
  HAL_RCC_DeInit();

  /* Clear Interrupt Enable Register & Interrupt Pending Register */
  for (uint8_t i = 0; i < (MCU_IRQS + 31u) / 32; i++) {
    NVIC->ICER[i] = 0xFFFFFFFF;
    NVIC->ICPR[i] = 0xFFFFFFFF;
  }

  // REMAP
  SYSCFG->MEMRMP = 0x01;

  /* Re-enable all interrupts */
  __enable_irq();

  // Set the MSP
  __set_MSP((uint32_t)BOOTVTAB->Initial_SP);

  // Jump to app firmware
  BOOTVTAB->Reset_Handler();
}