#include "WaterLeakSensor.h"

WaterLeakSensor::WaterLeakSensor(int pin) : _pin(pin) {
    pinMode(_pin, INPUT);
}

bool WaterLeakSensor::isLeakDetected() {
    return digitalRead(_pin) == HIGH;
}