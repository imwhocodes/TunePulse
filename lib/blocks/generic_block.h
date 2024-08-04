/**
 * @file generic_modules.h
 * @brief Header file for the SIGMA architecture controller layer modules.
 *
 * This file contains the definitions for the generic Block
 * which form the core of the controller layer in the SIGMA architecture.
 */

#ifndef GENERIC_BLOCK_H
#define GENERIC_BLOCK_H

#include <USBDevice.h>
#include <float.h>
#include <inttypes.h>
#include "control_modes.h"

#include "data_types.h"

// ################### GENERIC BLOCK #################################

/**
 * @brief Base class for all elements of the Controller layer in SIGMA
 * architecture.
 *
 * The Block class serves as the base class for all math functions.
 * It defines a pure virtual function tick(), which is the only block callback
 * and must be implemented by derived classes.
 */

class Block {
   public:
    /**
     * @brief Pure virtual function of Block callback.
     */
    virtual void tick() = 0;
};

// ################### MACROS #################################

/**
 * @brief Macro to define an input channel in a Block.
 * This macro creates a protected const reference to an input variable.
 * @param type The data type of the input.
 * @param name The name of the input variable.
 */
#define BLOCK_INPUT(type, name) \
   protected:                   \
    const type& name##_;

/**
 * @brief Macro to define an output channel in a Block.
 * This macro creates a protected reference to an output variable.
 * @param type The data type of the output.
 * @param name The name of the output variable.
 */
#define BLOCK_OUTPUT(type, name)     \
   protected:                        \
    type name##_;                    \
                                     \
   public:                           \
    const type& get_##name() const { \
        return name##_;              \
    }

// ################### BLOCK IO #################################

template <typename T>
class Input;

template <typename T>
class Output {
    T value;

   public:
    constexpr Output<T>(T&& input) : value{input} {}

    constexpr operator T&() { return this->value; }

    constexpr T& operator=(T&& new_value) {
        this->value = new_value;
        return this->value;
    };

    // constexpr T& operator[](int i) { this->value[i] }

    constexpr Input<T> asInput() { return this->value; }

    friend Input<T>;
};

template <typename T>
class Input {
    const T& value;

   public:
    constexpr Input<T>(const T& as_input) : value{as_input} {}
    constexpr Input<T>(const Output<T>& as_input) : value{as_input.value} {}
    constexpr operator const T&() const { return this->value; }
    constexpr const T& asValue() const { return this->value; }
};

template <typename T>
using Variable = Output<T>;

// void test() {
//     Output<int> v(0);

//     Input<int> i(v);

//     v = 5;

//     // i = 0;
// }

#endif  // GENERIC_BLOCK_H