#include "Relay.h"

Relay::Relay(int pin) : relayPin(pin) {
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW); // Ensure relay is off initially
}

void Relay::turnOn() {
    digitalWrite(relayPin, HIGH); // Turn the relay on
}

void Relay::turnOff() {
    digitalWrite(relayPin, LOW); // Turn the relay off
}