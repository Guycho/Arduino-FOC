#include "DshotDriver.h"

std::map<int, DshotDriver*> DshotDriver::instances;
int DshotDriver::instance_counter = 0;

DshotDriver::DshotDriver(uint8_t dshot_pin) {
    m_dshot_pin = dshot_pin;
    // default power-supply value
    voltage_power_supply = DEF_POWER_SUPPLY;
    voltage_limit = NOT_SET;
    pwm_frequency = NOT_SET;

    instance_id = instance_counter++;
    instances[instance_id] = this;
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
    m_timer.attach_us(1000 / 300, std::bind(&DshotDriver::sendDshotCommandStatic, instance_id));
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

    // Convert voltages to DShot values
    phase_output[0] = static_cast<uint16_t>(Ua / voltage_power_supply * 1000);
    phase_output[1] = static_cast<uint16_t>(Ub / voltage_power_supply * 1000);
    phase_output[2] = static_cast<uint16_t>(Uc / voltage_power_supply * 1000);
}

// Static function to send DShot command
void DshotDriver::sendDshotCommandStatic(int instance_id) {
    // Call the non-static member function
    if (instances.find(instance_id) != instances.end()) {
        instances[instance_id]->sendDshotCommand();
    }
}

// Non-static function to send DShot command
void DshotDriver::sendDshotCommand() {
    m_motor->send_dshot_value(phase_output[phase_index]);
    phase_index = (phase_index + 1) % 3;
}