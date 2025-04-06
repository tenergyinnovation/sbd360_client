#ifndef LORAMODULE_H
#define LORAMODULE_H

#include <Arduino.h>
#include <SPI.h>

class LoRaModule {
public:
    LoRaModule(uint8_t csPin, uint8_t resetPin, uint8_t irqPin);
    void begin();
    void sendData(const String &data);
    String receiveData();

private:
    uint8_t _csPin;
    uint8_t _resetPin;
    uint8_t _irqPin;
    void reset();
    void configure();
};

#endif // LORAMODULE_H