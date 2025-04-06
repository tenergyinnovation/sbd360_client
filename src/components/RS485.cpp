#include "RS485.h"

RS485::RS485(int txPin, int rxPin) : _txPin(txPin), _rxPin(rxPin) {
}

void RS485::begin(long baudRate) {
    Serial1.begin(baudRate, SERIAL_8N1, _rxPin, _txPin);
}

void RS485::send(const uint8_t* data, size_t length) {
    Serial1.write(data, length);
}

int RS485::receive(uint8_t* buffer, size_t bufferSize) {
    size_t bytesRead = 0;
    while (Serial1.available() && bytesRead < bufferSize) {
        buffer[bytesRead++] = Serial1.read();
    }
    return bytesRead;
}