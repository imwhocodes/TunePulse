/**
 * @file communication_channel_macros.h
 * @brief Provides macros for secure initialization and access control of communication channels.
 *
 * This module ensures that write access to communication channels is controlled to prevent accidental changes to sensitive areas.
 * All checks are performed at compile time, which means there is no impact on the final code size compared to simple read and write operations.
 * This protection is essential to avoid inadvertent errors in the system.
 */

#ifndef MODULE_CHANNEL_MACROS_H
#define MODULE_CHANNEL_MACROS_H

#include <type_traits>  // Includes the type_traits library, which provides templates for compile-time type information
#include "communication_channel_macros.h"

enum MODULE_CHANNEL_GROUP {
    CHANNEL_INPUT,
    CHANNEL_OUTPUT,
    CHANNEL_INPUT_OUTPUT
};

// Macro to initialize the module channel
#define MODULE_CHANNEL_INIT(type, var, group) \
    type& var##_link; \
    type var; \
    static constexpr MODULE_CHANNEL_GROUP var##_group = group

// Macro to write to the module channel with access check
#define MODULE_CHANNEL_WRITE(var) \
    static_assert(var##_group != CHANNEL_INPUT, "Error: Attempt to write to a read-only module channel"); \
    var##_link = var

// Macro to read from the module channel with access check
#define MODULE_CHANNEL_READ(var) \
    static_assert(var##_group != CHANNEL_OUTPUT, "Error: Attempt to read from a write-only module channel"); \
    var = var##_link

#endif  // MODULE_CHANNEL_MACROS_H
