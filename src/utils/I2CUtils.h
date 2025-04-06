#ifndef I2CUTILS_H
#define I2CUTILS_H

#include <Wire.h>

class I2CUtils {
public:
    static void begin(uint8_t address);
    static void writeByte(uint8_t address, uint8_t data);
    static uint8_t readByte(uint8_t address);
    static void writeBytes(uint8_t address, uint8_t* data, size_t length);
    static void readBytes(uint8_t address, uint8_t* buffer, size_t length);
};

#endif // I2CUTILS_H