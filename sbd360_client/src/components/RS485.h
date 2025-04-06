#ifndef RS485_H
#define RS485_H

#include <Arduino.h>

class RS485 {
public:
    RS485(int txPin, int rxPin);
    void begin(long baudRate);
    void send(const uint8_t* data, size_t length);
    size_t receive(uint8_t* buffer, size_t bufferSize);

private:
    int _txPin;
    int _rxPin;
};

#endif // RS485_H