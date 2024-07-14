/**
 * @file generic_modules.h
 * @brief Header file for the SIGMA architecture controller layer modules.
 *
 * This file contains the definitions for the generic Module, ComputeModule,
 * and SelectorModule classes, which form the core of the controller layer
 * in the SIGMA architecture.
 */

#ifndef GENERIC_MODULES_H
#define GENERIC_MODULES_H

#define OUTPUT(type, name)                                                                                             \
  protected:                                                                                                           \
    type q_##name;                                                                                                     \
                                                                                                                       \
  public:                                                                                                              \
    const type &name;                                                                                                  \
                                                                                                                       \
  private:

#define INPUT(type, name)                                                                                              \
  private:                                                                                                             \
    const type &name;

#define MODULE(name, initialization)                                                                                   \
    static auto name = initialization;                                                                                 \
    name.tick();

#endif // GENERIC_MODULES_H
