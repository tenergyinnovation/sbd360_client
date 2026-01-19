/***********************************************************************
 * Project      :     tenergy32hub
 * Description  :     Library for Tenergy32 Hub
 *                    This library provides functions to control various
 *                    peripherals and sensors on the Tenergy32 Hub board.
 * Hardware     :     tenergy32hub
 * Author       :     Tenergy Innovation Co., Ltd.
 * Date         :     27/04/2025
 * Revision     :     1.9.0
 * Rev1.0       :     Original
 * Rev1.1       :     Add Example for LoRa receive test [2025-05-02]
 * Rev1.2       :     Add showLibraryVersion() function [2025-05-03]
 * Rev1.3       :     Add clearOLED() function [2025-05-15]
 * Rev1.4       :     - Revise for begin function to show OLED messages [2025-05-31]
 *                    - Add function battery percentage and voltage reading
 * Rev1.5       :     - Add Example for tenergy32hub_readBattery [2025-05-31]
 * Rev1.6       :     Add LCD display functions [2025-06-01]
 * Rev1.7       :     Add SDM120 Modbus functions [2025-06-29]
 * Rev1.8       :     Add built-in Debounce functionality for SW1 and SW2 [2025-08-12]
 *                    - Implemented internal debounce logic (50ms delay) without external library
 *                    - Added getSW1Count(), getSW2Count(), resetSW1Count(), resetSW2Count()
 *                    - Enhanced switch reading with built-in debounce mechanism
 * Rev1.8.1     :     - Fix readSW1() and readSW2() to avoid double counting issue [2026-01-17 14:16]
 * Rev1.9.0     :     - Add option to initialize ADS1115 and/or LoRa in begin() [2026-01-19 10:12]
 * Rev1.10.0    :     - Add ModbusRTU Relay Module functions [2026-02-09 11:00]
 *                    - Add pin RX3 and PIN_RX3m PIN_TX3 for HardwareSerial
 * website      :     http://www.tenergyinnovation.co.th
 * Email        :     uten.boonliam@tenergyinnovation.co.th
 * TEL          :     +66 89-140-7205
 ***********************************************************************/
#ifndef TENERGY32HUB_H
#define TENERGY32HUB_H

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_SSD1306.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_ADS1X15.h>
#include <Ticker.h> // Include Ticker library
#include <HardwareSerial.h>

// User initialization options for begin() function
#define USER_NONE          0
#define USER_ADS1115       1
#define USER_LORA          2
#define USER_ADS1115_LORA  3

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
#define PIN_RX3 27
#define PIN_TX3 26
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

// --- SDM120 Modbus Register Address ---
#define SDM120_REG_VOLTAGE        0x0000
#define SDM120_REG_CURRENT        0x0006
#define SDM120_REG_ACTIVE_POWER   0x000C
#define SDM120_REG_APP_POWER      0x0012
#define SDM120_REG_POWER_FACTOR   0x0018
#define SDM120_REG_FREQUENCY      0x0046
#define SDM120_REG_IMPORT_ENERGY  0x0048

class Tenergy32Hub
{
public:
    const String _version = "1.9.0"; // Library version

public:
    Tenergy32Hub();

    // Show the library version
    void showLibraryVersion();

    // Initialize the Tenergy32 Hub board
    bool begin(uint8_t userOptions = USER_NONE, uint32_t loraFreq = 443E6);

    // Switches and sensors
    bool readSlideSwitch();
    bool readSW1();
    bool readSW2();
    unsigned int getSW1Count();    // Get number of times SW1 was pressed
    unsigned int getSW2Count();    // Get number of times SW2 was pressed
    void resetSW1Count();          // Reset SW1 press count
    void resetSW2Count();          // Reset SW2 press count
    int readMotionSensor();
    int readWaterLeak();

    // Actuators
    void relayOn();
    void relayOff();
    void setRelay(bool state);
    bool readRelayState();
    void setBlueLED(bool on);
    void setRedLED(bool on);
    void setbuildingLED(bool on);
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

    // clearOLED
    // Clears the OLED display and resets the cursor position.
    // This function is useful for preparing the display for new content.
    void clearOLED();

    bool initLCD(uint8_t address = LCD_ADDRESS, uint8_t cols = 16, uint8_t rows = 2); // Initializes the LCD display with specified address, columns, and rows.
    void displayLCD(const char *text, uint8_t col = 0, uint8_t row = 0);
    void clearLCD(); // Clears the LCD display and resets the cursor position.
    void onBacklightLCD(); // Turns on the backlight of the LCD display.
    void offBacklightLCD(); // Turns off the backlight of the LCD display.

    bool initADC(uint8_t address = ADS1115_ADDRESS);
    int16_t readADCChannel(uint8_t chan);
    int16_t readPotentiometer();

    // Play Mario theme on the buzzer.
    void marioSound();
    // Play Angry Bird sound on the buzzer.
    void angryBirdSound();

    // New: Blink functions for each LED using Ticker callbacks.
    // intervalMillis: full blink cycle duration.
    // If 0 is passed, blinking stops and the LED is turned off.
    void blinkRedLED(uint32_t intervalMillis);
    void blinkBlueLED(uint32_t intervalMillis);
    void blinkbuildingLED(uint32_t intervalMillis);

    // วัดเปอร์เซนต์แบตเตอรี่ (0-100%)
    bool readBattery_SOC(float &batteryVoltage, float &soc); // Returns battery percentage (0-100%)

    // SDM120 Modbus functions
    bool readSDM120Float(uint8_t slaveAddr, uint16_t regAddr, float &value, HardwareSerial &serial = Serial2, uint32_t baud = 9600);
    bool readSDM120All(uint8_t slaveAddr, float &voltage, float &current, float &activePower, float &importEnergy, HardwareSerial &serial = Serial2, uint32_t baud = 9600);
    bool readSDM120All(uint8_t slaveAddr, float &voltage, float &current, float &activePower, float &importEnergy, float &powerFactor, float &frequency, HardwareSerial &serial = Serial2, uint32_t baud = 9600);
    bool readSDM120Voltage(uint8_t slaveAddr, float &voltage, HardwareSerial &serial = Serial2, uint32_t baud = 9600);
    bool readSDM120Current(uint8_t slaveAddr, float &current, HardwareSerial &serial = Serial2, uint32_t baud = 9600);
    bool readSDM120ActivePower(uint8_t slaveAddr, float &activePower, HardwareSerial &serial = Serial2, uint32_t baud = 9600);
    bool readSDM120ImportEnergy(uint8_t slaveAddr, float &importEnergy, HardwareSerial &serial = Serial2, uint32_t baud = 9600);
    bool readSDM120PowerFactor(uint8_t slaveAddr, float &powerFactor, HardwareSerial &serial = Serial2, uint32_t baud = 9600);
    bool readSDM120Frequency(uint8_t slaveAddr, float &frequency, HardwareSerial &serial = Serial2, uint32_t baud = 9600);  

private:
    Adafruit_SSD1306 *_oled;
    LiquidCrystal_I2C *_lcd;
    Adafruit_ADS1115 *_ads;

    // Debounce variables for SW1
    bool _sw1_state;
    bool _sw1_lastState;
    bool _sw1_lastReading;
    unsigned long _sw1_lastDebounceTime;
    unsigned int _sw1_count;
    
    // Debounce variables for SW2
    bool _sw2_state;
    bool _sw2_lastState;
    bool _sw2_lastReading;
    unsigned long _sw2_lastDebounceTime;
    unsigned int _sw2_count;
    
    // Debounce delay in milliseconds
    static const unsigned long DEBOUNCE_DELAY = 50;

    // Private helper functions for updating switch states
    void _updateSW1State();
    void _updateSW2State();

    // Private Ticker objects for asynchronous LED blinking.
    Ticker _tickerRed;
    Ticker _tickerBlue;
    Ticker _tickerBuilding;

    // Private state variables for LED toggling.
    bool _redState;
    bool _blueState;
    bool _buildingState;

    // RelayModusRTU pins (จำไว้เพื่อใช้ในฟังก์ชั่นอื่น ๆ)
    uint8_t _relayRTU_rx;
    uint8_t _relayRTU_tx;
    uint8_t _relayRTU_port;  // 1 for rs485(1), 2 for rs485_2(2)

    // Static pointer to allow callbacks to access this instance.
    static Tenergy32Hub *_instance;

    // Static callback functions used by Ticker (for each LED).
    static void redLEDToggle();
    static void blueLEDToggle();
    static void buildingLEDToggle();

    // ModbusRTU Relay Module 
    bool RelayModusRTU_begin(uint8_t rx = PIN_RX3, uint8_t tx = PIN_TX3);
    int8_t RelayModusRTU_searchAddress(uint8_t startID = 1, uint8_t stopID = 20);
    bool RelayModusRTU_Control(uint8_t address = 1, uint8_t channel = 1, bool state = true);
    bool RelayModusRTU_Status(uint8_t address = 1, uint8_t channel = 1);
};

#endif // TENERGY32HUB_H
