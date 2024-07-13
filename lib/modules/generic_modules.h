/**
 * @file generic_modules.h
 * @brief Header file for the SIGMA architecture controller layer modules.
 *
 * This file contains the definitions for the
 * generic Module, ComputeModule, and SelectorModule classes,
 * which form the core of the controller layer in the SIGMA architecture.
 */

#ifndef GENERIC_MODULES_H
#define GENERIC_MODULES_H

#include <inttypes.h>

// ################### INTERUPTS CONTROL #################################

/**
 * @brief Enables IRQ (Interrupt Requests).
 */
void enable_IRQ();

/**
 * @brief Disables IRQ (Interrupt Requests).
 */
void disable_IRQ();

// ################### GENERIC MODULE #################################

/**
 * @brief Base interface for all elements of the Controller layer in SIGMA
 * architecture.
 *
 * The Module class serves as the base class for all controller elements.
 * It defines a pure virtual function tick(), which is the main module callback
 * and must be implemented by derived classes.
 */
class Module {
 public:
  /**
   * @brief Pure virtual function of Module callback.
   *
   * This function should be implemented by derived classes.
   */
  virtual void tick() = 0;
};

// ################### COMPUTE MODULE #################################

/**
 * @brief Interface for modules that perform computations.
 *
 * The ComputeModule class is a specialized type of Module that performs
 * computations on data from input channels and writes results to output
 * channels. It should have minimal internal state and use Computation Blocks to
 * perform calculations. Decision-making logic should be avoided in this class.
 */
class ComputeModule : public Module {
 protected:
  /**
   * @brief Reads data from input channels.
   */
  virtual void read() = 0;

  /**
   * @brief Performs computations on the read data.
   */
  virtual void compute() = 0;

  /**
   * @brief Writes the computed data to output channels.
   */
  virtual void write() = 0;

 public:
  /**
   * @brief Executes the read-compute-write cycle.
   * Disables IRQs during read and write operations to ensure data integrity.
   */
  void tick() override {
    disable_IRQ();
    read();
    enable_IRQ();
    compute();
    disable_IRQ();
    write();
    enable_IRQ();
  }
};

// ################### SELECTOR MODULE #################################

/**
 * @brief Interface for modules that allow switching between different
 operational modes.

 * The SelectorModule class is a specialized type of Module that allows discrete
 changes
 * in system behavior by switching between Modules that share the same output
 channels.
 * Only one internal Module can be active at any moment.
 */
class SelectorModule : public Module {
 protected:
  uint32_t& selectorChannel;  ///< Reference to an integer variable used to
                              ///< select the module.
  Module* activeModule;  ///< Pointer to the active module (default is nullptr).

  /**
   * @brief Selects the active internal Module.
   *
   * This function must be implemented by derived classes to determine the
   * active module based on the selectorChannel value.
   *
   * @return Pointer to the selected Module.
   */
  virtual Module* Select() = 0;

 public:
  /**
   * @brief Constructor for SelectorModule.
   *
   * @param Channel Reference to an integer variable used for selecting the
   * module.
   */
  SelectorModule(uint32_t& Channel)
      : selectorChannel(Channel), activeModule(nullptr) {}

  /**
   * @brief Executes the selection of the active internal Module.
   */
  void tick() override {
    activeModule = Select();
    if (activeModule != nullptr)
      activeModule->tick();
  }
};

#endif  // GENERIC_MODULES_H