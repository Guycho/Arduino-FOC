#include "DshotDriver.h"

DshotDriver::DshotDriver(uint8_t dshot_pin, uint8_t rmt_channel) {
    m_dshot_pin = dshot_pin;
    m_rmt_channel = rmt_channel;
    // default power-supply value
    voltage_power_supply = DEF_POWER_SUPPLY;
    voltage_limit = NOT_SET;
    pwm_frequency = NOT_SET;
}

// enable motor driver
void DshotDriver::enable() {
    setPwm(0, 0, 0);
}

// disable motor driver
void DshotDriver::disable() {
    setPwm(0, 0, 0);
}

// init hardware pins
int DshotDriver::init() {
    motor = new DShotRMT(m_dshot_pin, m_rmt_channel);
    motor->begin(DSHOT_MODE);
    return 1;
}

// Set voltage to the pwm pin
void DshotDriver::setPhaseState(PhaseState sa, PhaseState sb, PhaseState sc) {
    // disable if needed
}

// Set voltage to the pwm pin
void DshotDriver::setPwm(float Ua, float Ub, float Uc) {
    // limit the voltage in driver
    Ua = _constrain(Ua, 0.0f, voltage_limit);
    Ub = _constrain(Ub, 0.0f, voltage_limit);
    Uc = _constrain(Uc, 0.0f, voltage_limit);
    // calculate duty cycle
    // limited in [0,1]
    dc_a = _constrain(Ua / voltage_power_supply, 0.0f, 1.0f);
    dc_b = _constrain(Ub / voltage_power_supply, 0.0f, 1.0f);
    dc_c = _constrain(Uc / voltage_power_supply, 0.0f, 1.0f);

    uint16_t scaled_dc_a = dc_a * 663;
    uint16_t scaled_dc_b = dc_b * 663;
    uint16_t scaled_dc_c = dc_c * 663;
}

void DshotDriver::sendDshotCommand(uint16_t command1, uint16_t command2, uint16_t command3) {
    motor->sendThrottleValue(50 + command1);
    motor->sendThrottleValue(50 + 664 + command2);
    motor->sendThrottleValue(50 + 664 + 664 +command3);
    // Dshot command sending
}
