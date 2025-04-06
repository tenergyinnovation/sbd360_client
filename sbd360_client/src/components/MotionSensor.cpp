#include "MotionSensor.h"

MotionSensor::MotionSensor(int pin) : sensorPin(pin) {
    pinMode(sensorPin, INPUT);
}

bool MotionSensor::isMotionDetected() {
    return digitalRead(sensorPin) == HIGH;
}