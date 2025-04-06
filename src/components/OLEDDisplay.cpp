#include "OLEDDisplay.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

OLEDDisplay::OLEDDisplay() {}

void OLEDDisplay::init() {
    display.begin(SSD1306_I2C_ADDRESS, OLED_RESET);
    display.clearDisplay();
    display.display();
}

void OLEDDisplay::displayText(const String &text) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(text);
    display.display();
}

void OLEDDisplay::clearDisplay() {
    display.clearDisplay();
    display.display();
}