#include "SPIUtils.h"
#include <SPI.h>

void SPIUtils::begin() {
    SPI.begin();
}

void SPIUtils::end() {
    SPI.end();
}

void SPIUtils::setClockDivider(uint8_t divider) {
    SPI.setClockDivider(divider);
}

void SPIUtils::setDataMode(uint8_t mode) {
    SPI.setDataMode(mode);
}

void SPIUtils::setBitOrder(uint8_t order) {
    SPI.setBitOrder(order);
}

uint8_t SPIUtils::transfer(uint8_t data) {
    return SPI.transfer(data);
}

void SPIUtils::transfer(uint8_t* data, size_t length) {
    SPI.transfer(data, length);
}