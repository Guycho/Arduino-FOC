#ifndef DSHOTDRIVER_H
#define DSHOTDRIVER_H

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
     * @param phA A phase pin
     * @param phB B phase pin
     * @param phC C phase pin
     * @param en1 enable pin (optional input)
     * @param en2 enable pin (optional input)
     * @param en3 enable pin (optional input)
     */
    DshotDriver(int dshot_pin);

    /** Motor hardware init function */
    int init() override;
    /** Motor disable function */
    void disable() override;
    /** Motor enable function */
    void enable() override;

    // hardware variables
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
    DShotRMT *motor;
    const auto DSHOT_MODE = DSHOT300;
    uint8_t m_dshot_pin;    //!< DShot pin number
    uint8_t m_rmt_channel;  //!< RMT channel number
    void sendDshotCommand(uint16_t command1, uint16_t command2, uint16_t command3);
  };

#endif