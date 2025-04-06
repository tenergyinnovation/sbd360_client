#include "I2CUtils.h"
#include <Wire.h>

void I2CUtils::begin() {
    Wire.begin();
}

void I2CUtils::writeByte(uint8_t address, uint8_t data) {
    Wire.beginTransmission(address);
    Wire.write(data);
    Wire.endTransmission();
}

uint8_t I2CUtils::readByte(uint8_t address) {
    Wire.requestFrom(address, (uint8_t)1);
    if (Wire.available()) {
        return Wire.read();
    }
    return 0; // Return 0 if no data is available
}

void I2CUtils::writeBytes(uint8_t address, uint8_t* data, size_t length) {
    Wire.beginTransmission(address);
    Wire.write(data, length);
    Wire.endTransmission();
}

void I2CUtils::readBytes(uint8_t address, uint8_t* buffer, size_t length) {
    Wire.requestFrom(address, length);
    size_t index = 0;
    while (Wire.available() && index < length) {
        buffer[index++] = Wire.read();
    }
}