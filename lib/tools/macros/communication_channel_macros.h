/**
 * @file communication_channel_macros.h
 * @brief Provides macros for secure initialization and access control of communication channels.
 *
 * This module ensures that write access to communication channels is controlled to prevent accidental changes to sensitive areas.
 * All checks are performed at compile time, which means there is no impact on the final code size compared to simple read and write operations.
 * This protection is essential to avoid inadvertent errors in the system.
 */

#ifndef COMMUNICATION_CHANNEL_MACROS_H
#define COMMUNICATION_CHANNEL_MACROS_H

#include <type_traits>  // Includes the type_traits library, which provides templates for compile-time type information
#include "..\..\system_layout\communication_channels_access_keys.h"

// ######### Macros for channel initialization with default value handling ############

/**
 * @brief Macro to initialize a variable with a default value.
 * This macro also stores the default value for future reference.
 * @param type The type of the variable.
 * @param var The variable to initialize.
 * @param def_val The default value to initialize the variable with.
 */
#define CHANNEL_INIT_DEFAULT(type, var, def_val) \
  constexpr type var##_DEFAULT = def_val;        \
  type var = def_val;

/**
 * @brief Macro to set the variable to its default value.
 * @param var The variable to set to its default value.
 */
#define CHANNEL_SET_DEFAULT(var) var = var##_DEFAULT;

/**
 * @brief Macro to check if the variable's value is equal to its default value.
 * @param var The variable to check.
 * @return true if the variable's value is equal to its default value, false otherwise.
 */
#define CHANNEL_IS_DEFAULT(var) (var == var##_DEFAULT)

// ######### Macros for channel write access control ############

/**
 * @brief Template metaprogramming to check if an access key is in the list.
 * This structure uses variadic templates to accept an arbitrary number of
 * AccessKey values and provides a constexpr function to check if a given
 * key is part of the list.
 * @tparam Keys The list of access keys.
 */
template <AccessKey... Keys>
struct AccessList {
  /**
   * @brief Checks if the given access key is in the list.
   * @param key The access key to check.
   * @return true if the key is in the list, false otherwise.
   */
  static constexpr bool contains(AccessKey key) {
    return ((key == Keys) || ...);  // Fold expression to check if 'key' matches any of the 'Keys'
  }
};

// ############### Macros for access control #################

/**
 * @brief Macro to set access keys for a variable.
 * Creates a constexpr instance of AccessList with given access keys.
 * @param var The variable to set access keys for.
 * @param ... The access keys to be granted for the variable [multiple comma-separated possible]
 */
#define CHANNEL_WRITE_ACCESS(var, ...) \
  constexpr auto var##_access = AccessList<__VA_ARGS__>{};

/**
 * @brief Macro to read the value of a variable.
 * Simply returns the value of the variable.
 * @param var The variable to read the value from.
 * @return The value of the variable.
 */
#define CHANNEL_READ(var) (var)

/**
 * @brief Macro to write a value to a variable with access control.
 * Checks if the access key is allowed, and if so, assigns 'val' to 'var'.
 * Otherwise, triggers a compile-time error.
 * @param var The variable to write the value to.
 * @param val The value to be written.
 * @param key The access key to authorize the write operation.
 */
#define CHANNEL_WRITE(var, val, key)                                 \
  do {                                                               \
    if (key == GOD_MODE || var##_access.contains(key)) {             \
      var = val;                                                     \
    } else {                                                         \
      static_assert(var##_access.contains(key) || key == GOD_MODE,   \
                    "Write access denied for the given access key"); \
    }                                                                \
  } while (0)


// ######### Macros for channel write access control ############

/**
 * @brief Macro to fully initialize a Communication Channel with default values and
 * access keys.
 * Combines CHANNEL_INIT_DEFAULT(type, var, def_val) and CHANNEL_WRITE_ACCESS(var, ...)
 * @param type The type of the variable.
 * @param var The variable to initialize.
 * @param def_val The default value to initialize the variable with.
 * @param ... The access keys to be granted for the variable [multiple comma-separated possible]
 */
#define CHANNEL_INIT(type, var, def_val, ...) \
  CHANNEL_INIT_DEFAULT(type, var, def_val);   \
  CHANNEL_WRITE_ACCESS(var, __VA_ARGS__);

#endif  // COMMUNICATION_CHANNEL_MACROS_H
