#pragma once

#include "Module.h"
#include "Tickable.h"
#include <inttypes.h>

class WriteGroupVOLTG_D_Q : public Tickable
{
    // protected:
    //   int32_t q_VOLTG_Q, q_VOLTG_D;

    // public:
    //   const int32_t &VOLTG_Q, &VOLTG_D;
    OUTPUT(int32_t, VOLTG_D)
    OUTPUT(int32_t, VOLTG_Q)

  public:
    WriteGroupVOLTG_D_Q() : VOLTG_Q(q_VOLTG_Q), VOLTG_D(q_VOLTG_D)
    {
    }
};

class VoltageEstimator : public WriteGroupVOLTG_D_Q
{
    INPUT(int32_t, CURNT_D_REF)
    INPUT(int32_t, CURNT_Q_REF)

  public:
    VoltageEstimator(const int32_t &i_CURNT_Q_REF, const int32_t &i_CURNT_D_REF)
        : CURNT_Q_REF(i_CURNT_Q_REF), CURNT_D_REF(i_CURNT_D_REF)
    {
    }

    virtual void tick() override
    {
        q_VOLTG_Q = 0;
        q_VOLTG_D = 20;
    }
};

class CurrentEstimator : public WriteGroupVOLTG_D_Q
{
    INPUT(int32_t, CURNT_D_REF)
    INPUT(int32_t, CURNT_Q_REF)

  public:
    CurrentEstimator(const int32_t &i_CURNT_Q_REF, const int32_t &i_CURNT_D_REF)
        : CURNT_Q_REF(i_CURNT_Q_REF), CURNT_D_REF(i_CURNT_D_REF)
    {
    }

    virtual void tick() override
    {
        q_VOLTG_Q = -1234;
        q_VOLTG_D = 255;
    }
};

enum SelectorEstimatorModes
{
    voltageEstimationMode,
    currentFeedbackMode,
};

class SelectorEstimator : public WriteGroupVOLTG_D_Q
{
  private:
    WriteGroupVOLTG_D_Q **modules;

    INPUT(SelectorEstimatorModes, MODE)
    INPUT(int32_t, CURNT_Q_REF)
    INPUT(int32_t, CURNT_D_REF)

  public:
    SelectorEstimator(const SelectorEstimatorModes &i_MODE, const int32_t &i_CURNT_Q_REF, const int32_t &i_CURNT_D_REF)
        : MODE(i_MODE), CURNT_Q_REF(i_CURNT_Q_REF), CURNT_D_REF(i_CURNT_D_REF)
    {
        static VoltageEstimator voltageEstimationMode(CURNT_Q_REF, CURNT_D_REF);
        static CurrentEstimator currentFeedbackMode(CURNT_Q_REF, CURNT_D_REF);

        static WriteGroupVOLTG_D_Q *_modules[] = {&voltageEstimationMode, &currentFeedbackMode};
        modules = _modules;
    }

    void tick() override
    {
        modules[MODE]->tick();
        q_VOLTG_D = modules[MODE]->VOLTG_D;
        q_VOLTG_Q = modules[MODE]->VOLTG_Q;
    }
};
