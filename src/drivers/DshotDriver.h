#ifndef DSHOTDRIVER_H
#define DSHOTDRIVER_H

#include <Ticker.h>

#include <map>

#include "../common/base_classes/BLDCDriver.h"
#include "../common/defaults.h"
#include "../common/foc_utils.h"
#include "../common/time_utils.h"
#include "DShotRMT.h"
#include "hardware_api.h"

/**
 * DShot BLDC driver class
 */
class DshotDriver : public BLDCDriver {
   public:
    /**
     * DshotDriver class constructor
     * @param dshot_pin DShot pin
     */
    DshotDriver(uint8_t dshot_pin);

    /** Motor hardware init function */
    int init() override;
    /** Motor disable function */
    void disable() override;
    /** Motor enable function */
    void enable() override;

    /**
     * Set phase voltages to the hardware using DShot protocol
     *
     * @param Ua - phase A voltage
     * @param Ub - phase B voltage
     * @param Uc - phase C voltage
     */
    void setPwm(float Ua, float Ub, float Uc) override;

    /**
     * Set phase states to the hardware
     *
     * @param sa - phase A state : active / disabled ( high impedance )
     * @param sb - phase B state : active / disabled ( high impedance )
     * @param sc - phase C state : active / disabled ( high impedance )
     */
    void setPhaseState(PhaseState sa, PhaseState sb, PhaseState sc) override;

   private:
    uint8_t m_dshot_pin;
    DShotRMT* m_motor;
    uint16_t phase_output[3];
    uint8_t phase_index = 0;
    Ticker m_timer;

    static std::map<int, DshotDriver*> instances;
    static int instance_counter;
    int instance_id;

    static void sendDshotCommandStatic(int instance_id);
    void sendDshotCommand();
};

#endif #ifndef DSHOTDRIVER_H
#define DSHOTDRIVER_H

#include <Ticker.h>

#include <map>

#include "../common/base_classes/BLDCDriver.h"
#include "../common/defaults.h"
#include "../common/foc_utils.h"
#include "../common/time_utils.h"
#include "DShotRMT.h"
#include "hardware_api.h"

/**
 * DShot BLDC driver class
 */
class DshotDriver : public BLDCDriver {
   public:
    /**
     * DshotDriver class constructor
     * @param dshot_pin DShot pin
     */
    DshotDriver(uint8_t dshot_pin);

    /** Motor hardware init function */
    int init() override;
    /** Motor disable function */
    void disable() override;
    /** Motor enable function */
    void enable() override;

    /**
     * Set phase voltages to the hardware using DShot protocol
     *
     * @param Ua - phase A voltage
     * @param Ub - phase B voltage
     * @param Uc - phase C voltage
     */
    void setPwm(float Ua, float Ub, float Uc) override;

    /**
     * Set phase states to the hardware
     *
     * @param sa - phase A state : active / disabled ( high impedance )
     * @param sb - phase B state : active / disabled ( high impedance )
     * @param sc - phase C state : active / disabled ( high impedance )
     */
    void setPhaseState(PhaseState sa, PhaseState sb, PhaseState sc) override;

   private:
    uint8_t m_dshot_pin;
    DShotRMT* m_motor;
    uint16_t phase_output[3];
    uint8_t phase_index = 0;
    Ticker m_timer;
    const dshot_mode_e DSHOT_MODE = DSHOT300;
    static std::map<int, DshotDriver*> instances;
    static int instance_counter;
    int instance_id;

    static void sendDshotCommandStatic(int instance_id);
    void sendDshotCommand();
};

#endif