#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class OLEDDisplay {
public:
    OLEDDisplay(int width, int height, int resetPin);
    void init();
    void displayText(const String &text);
    void clearDisplay();

private:
    Adafruit_SSD1306 display;
};

#endif // OLED_DISPLAY_H