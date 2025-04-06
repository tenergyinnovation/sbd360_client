#include "Potentiometer.h"

Potentiometer::Potentiometer(int pin) : _pin(pin) {
    pinMode(_pin, INPUT);
}

int Potentiometer::readValue() {
    return analogRead(_pin);
}