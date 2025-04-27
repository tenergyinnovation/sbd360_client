// File: tenergy32hub.h
#ifndef TENERGY32HUB_H
#define TENERGY32HUB_H

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_SSD1306.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_ADS1X15.h>

// Pin definitions
#define PIN_SLIDE_SWITCH 36
#define PIN_SW1 34
#define PIN_SW2 35
#define PIN_MOTION_SENSOR 32
#define PIN_WATER_LEAK 33
#define PIN_RELAY 25
#define PIN_LORA_RESET 14
#define PIN_LED_BLUE 12
#define PIN_BUZZER 13
#define PIN_LORA_DIO0 15
#define PIN_CHARGER_RESET 2
#define PIN_LED_RED 4
#define PIN_RX_485 16
#define PIN_TX_485 17
#define PIN_LORA_NSS 5
#define PIN_LORA_SCK 18
#define PIN_LORA_MISO 19
#define PIN_I2C_SDA 21
#define PIN_I2C_SCL 22
#define PIN_LORA_MOSI 23

// Defaults for I2C devices
#define OLED_ADDRESS 0x3C
#define LCD_ADDRESS 0x27
#define ADS1115_ADDRESS 0x48

class Tenergy32Hub
{
public:
    Tenergy32Hub();
    bool begin(uint32_t loraFreq = 443E6);

    // Switches and sensors
    bool readSlideSwitch();
    bool readSW1();
    bool readSW2();
    int readMotionSensor();
    int readWaterLeak();

    // Actuators
    void relayOn();
    void relayOff();
    void setBlueLED(bool on);
    void setRedLED(bool on);
    void beep(uint8_t times, uint16_t ms = 100);

    // Charger reset
    void resetCharger();

    // LoRa functions
    bool sendLoRa(const uint8_t *data, size_t len);
    bool receiveLoRa(uint8_t *buffer, size_t maxLen, int &received);

    // RS485 (simple TX/RX)
    void rs485TransmitMode(bool enable);
    size_t sendRS485(const uint8_t *data, size_t len);
    size_t receiveRS485(uint8_t *buffer, size_t maxLen);

    // I2C devices
    void initI2C();
    bool initOLED(uint8_t address = OLED_ADDRESS);
    void displayOLED(const char *text);
    void displayOLEDInfo();


    // Displays text on up to 4 separate lines on the OLED.
    // Each parameter is optional (default empty string) and at least line1 should be provided.
    void displayOLEDLines(const char *line1, const char *line2 = "", const char *line3 = "", const char *line4 = "");

    bool initLCD(uint8_t address = LCD_ADDRESS);
    void displayLCD(const char *text, uint8_t col = 0, uint8_t row = 0);

    bool initADC(uint8_t address = ADS1115_ADDRESS);
    int16_t readADCChannel(uint8_t chan);
    int16_t readPotentiometer();
    
    // Play Mario theme on the buzzer.
    void marioSound();
  
     // Play Angry Bird sound on the buzzer.
     void angryBirdSound();

private:
    Adafruit_SSD1306 *_oled;
    LiquidCrystal_I2C *_lcd;
    Adafruit_ADS1115 *_ads;
};

#endif // TENERGY32HUB_H
