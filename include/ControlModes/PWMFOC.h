#pragma once

#include "Module.h"
#include "Tickable.h"
#include "VoltageCurrentMode.h"
#include <inttypes.h>

class VoltageAdjuster : public Tickable
{
    INPUT(int32_t, VOLTG_D)
    INPUT(int32_t, VOLTG_Q)

    OUTPUT(int32_t, VOLTG_D_ADJ)
    OUTPUT(int32_t, VOLTG_Q_ADJ)

  public:
    VoltageAdjuster(const int32_t &i_VOLTG_D, const int32_t &i_VOLTG_Q)
        : VOLTG_D(i_VOLTG_D), VOLTG_Q(i_VOLTG_Q), VOLTG_D_ADJ(q_VOLTG_D_ADJ), VOLTG_Q_ADJ(q_VOLTG_Q_ADJ)
    {
    }
    void tick() override
    {
        q_VOLTG_D_ADJ = VOLTG_D + 12;
        q_VOLTG_Q_ADJ = VOLTG_Q - 425;
    }
};

class PWMFOC_ControlMode : public Tickable
{
    INPUT(SelectorEstimatorModes, MODE)
    INPUT(int32_t, CURNT_D_REF)
    INPUT(int32_t, CURNT_Q_REF)

    SelectorEstimator sest = SelectorEstimator(MODE, CURNT_D_REF, CURNT_Q_REF);
    VoltageAdjuster vadj = VoltageAdjuster(sest.VOLTG_D, sest.VOLTG_Q);

    OUTPUT(int32_t, VOLTG_D_ADJ)
    OUTPUT(int32_t, VOLTG_Q_ADJ)

  public:
    PWMFOC_ControlMode(const SelectorEstimatorModes &i_MODE, const int32_t &i_CURNT_Q_REF, const int32_t &i_CURNT_D_REF)
        : MODE(i_MODE), CURNT_D_REF(i_CURNT_D_REF), CURNT_Q_REF(i_CURNT_Q_REF), VOLTG_D_ADJ(q_VOLTG_D_ADJ),
          VOLTG_Q_ADJ(q_VOLTG_Q_ADJ)
    {
    }

    void tick() override
    {
        sest.tick();
        vadj.tick();
        q_VOLTG_D_ADJ = vadj.VOLTG_D_ADJ;
        q_VOLTG_Q_ADJ = vadj.VOLTG_Q_ADJ;
    }
};
