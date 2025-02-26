#include "DshotDriver.h"

DshotDriver::DshotDriver(uint8_t dshot_pin) {
    m_dshot_pin = dshot_pin;
    // default power-supply value
    voltage_power_supply = DEF_POWER_SUPPLY;
    voltage_limit = NOT_SET;
    pwm_frequency = NOT_SET;
}

// enable motor driver
void DshotDriver::enable() { setPwm(0, 0, 0); }

// disable motor driver
void DshotDriver::disable() { setPwm(0, 0, 0); }

// init hardware pins
int DshotDriver::init() {
    m_motor = new DShotRMT(m_dshot_pin);
    m_motor->begin(DSHOT_MODE);
    long unsigned int now = micros();
    long unsigned int last = millis();
    while (true) {
        if (micros() - last > 1000 / 300) {
            m_motor->send_dshot_value(0);
        }
        if (millis() - now > 2000) {
            break;
        }
    }
    m_timer = new Chrono(Chrono::MICROS, true);
    initialized = 1;
    return 1;
}

// Set phase states to the hardware
void DshotDriver::setPhaseState(PhaseState sa, PhaseState sb, PhaseState sc) {
    // disable if needed
}

// Set phase voltages to the hardware using DShot protocol
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
    phase_output[0] = 50 + scaled_dc_a;
    phase_output[1] = 50 + 664 + scaled_dc_b;
    phase_output[2] = 50 + 664 + 664 + scaled_dc_c;
}

void DshotDriver::sendDshotCommand() {
    m_motor->send_dshot_value(phase_output[phase_index]);
    phase_index = (phase_index + 1) % 3;
}

void DshotDriver::run() {
    // run the motor
    if (m_timer->hasPassed(1000 / 300), true) {
        sendDshotCommand();
    }
}