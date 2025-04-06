#ifndef SPIUTILS_H
#define SPIUTILS_H

#include <SPI.h>

class SPIUtils {
public:
    static void begin();
    static void end();
    static void transfer(uint8_t* data, size_t length);
    static uint8_t transfer(uint8_t data);
};

#endif // SPIUTILS_H