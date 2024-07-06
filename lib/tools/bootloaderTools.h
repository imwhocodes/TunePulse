#ifndef BOOTLOADER_TOOLS_H
#define BOOTLOADER_TOOLS_H

// Libraries
#include "Arduino.h"

#define BOOT_ADDR \
  0x1FFF0000  // MCU bootloader address for STM32G431, need to change if needed
              // for other MCUs.
#define MCU_IRQS 71u  // Page number 28 in the reference manual for STM32G431
#define BOOTVTAB ((BootVectorTable*)BOOT_ADDR)

////////////////////////////////*
////* Struct definitions *//////
////////////////////////////////*

typedef struct {
  uint32_t* Initial_SP;
  void (*Reset_Handler)(void);
} BootVectorTable;

////////////////////////////////*
////* Function Prototypes *//////
////////////////////////////////*

/**
 * @brief Initiates a jump to the system bootloader.
 *
 * This function prepares the system to enter the bootloader mode, typically for
 * firmware updates. It performs necessary system cleanup and state preparation,
 * ensuring a safe transition to the bootloader. The specific actions taken can
 * include disabling interrupts, resetting peripheral states, and configuring
 * system clocks. It may also involve setting specific flags or registers that
 * the bootloader checks upon startup to determine whether to enter bootloader
 * mode or continue with normal application execution.
 *
 * Note: The actual implementation details and the necessity of certain actions
 * depend on the target microcontroller architecture and the bootloader's
 * requirements. This function may require customization for different hardware
 * setups.
 *
 * Usage:
 *     JumpToBootloader();
 */
void JumpToBootloader(void);

#endif  // BOOTLOADER_TOOLS_H