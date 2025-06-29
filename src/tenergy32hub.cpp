// File: tenergy32hub.cpp
#include "tenergy32hub.h"
#include <Ticker.h>
#include <HardwareSerial.h>

// Initialize static instance pointer to NULL.
Tenergy32Hub *Tenergy32Hub::_instance = nullptr;

/***********************************************************************
 * FUNCTION:    Tenergy32Hub
 * DESCRIPTION: Constructor for the Tenergy32Hub class.
 *              Initializes member variables and sets up the instance pointer.
 ***********************************************************************/
Tenergy32Hub::Tenergy32Hub() : _oled(nullptr), _lcd(nullptr), _ads(nullptr),
                               _redState(false), _blueState(false), _buildingState(false)
{
    _instance = this; // Assign the static instance pointer.
}

/***********************************************************************
 * FUNCTION:    showLibraryVersion
 * DESCRIPTION: Displays the library version on the serial monitor.
 * PARAMETERS:  none
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::showLibraryVersion()
{
    Serial.print("Tenergy32Hub Library Version: ");
    Serial.println(_version);
    if (_oled)
    {
        _oled->clearDisplay();
        _oled->setCursor(0, 0);
        _oled->println("Tenergy32Hub Library Version:");
        _oled->println(_version);
        _oled->display();
    }
}

/***********************************************************************
 * FUNCTION:    begin
 * DESCRIPTION: Initializes the Tenergy32Hub hardware.
 *              Sets up pin modes, initializes LoRa, I2C, and other
 *              peripherals.
 * PARAMETERS:  loraFreq - Frequency for LoRa communication (default is 433E6).
 * RETURNED:    true if initialization is successful, false otherwise.
 ***********************************************************************/
bool Tenergy32Hub::begin(uint32_t loraFreq)
{
    // Initialize serial communication
    Serial.begin(115200);
    Serial.println("Initializing Tenergy32Hub...");

    // Set up initial pin modes
    Serial.println("Setting pin modes...");
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);
    pinMode(PIN_CHARGER_RESET, OUTPUT);
    digitalWrite(PIN_CHARGER_RESET, HIGH);
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, LOW); // Turn off the built-in LED

    // Step 1: Initialize I2C
    Serial.println("Initializing I2C...");
    initI2C();

    // Step 2: Initialize OLED display
    Serial.println("Initializing OLED...");
    _oled = new Adafruit_SSD1306(128, 32, &Wire);
    if (!_oled->begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS))
    {
        Serial.println("fail to initial OLED");
        return false;
    }
    _oled->clearDisplay();
    if (_oled)
    {
        _oled->clearDisplay();
        _oled->setTextSize(1);
        _oled->setTextColor(SSD1306_WHITE);
        _oled->setCursor(0, 0);
        _oled->println("Init OLED...");
        _oled->display();
        vTaskDelay(300); // Delay to show the message
        _oled->setCursor(0, 10);
        _oled->println("OLED Init OK");
        _oled->display();
        vTaskDelay(1000); // Delay to show the message
    }

    // Step 3: Set up remaining peripheral pin modes
    Serial.println("Setting peripheral pin modes...");
    if (_oled)
    {
        _oled->clearDisplay();
        _oled->setCursor(0, 0);
        _oled->println("Setting peripherals...");
        _oled->display();
        vTaskDelay(300); // Delay to show the message
    }
    pinMode(PIN_SLIDE_SWITCH, INPUT);
    pinMode(PIN_SW1, INPUT_PULLUP);
    pinMode(PIN_SW2, INPUT_PULLUP);
    pinMode(PIN_MOTION_SENSOR, INPUT);
    pinMode(PIN_WATER_LEAK, INPUT);
    pinMode(PIN_RELAY, OUTPUT);
    pinMode(PIN_LED_BLUE, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_CHARGER_RESET, OUTPUT);
    if (_oled)
    {
        _oled->setCursor(0, 10);
        _oled->println("Peripherals OK");
        _oled->display();
        vTaskDelay(1000); // Delay to show the message
        _oled->clearDisplay();
    }

    // Step 4: Initialize LoRa
    Serial.println("Initializing LoRa...");
    if (_oled)
    {
        _oled->clearDisplay();
        _oled->setCursor(0, 0);
        _oled->println("Init LoRa...");
        _oled->display();
        vTaskDelay(300); // Delay to show the message
    }
    SPI.begin(PIN_LORA_SCK, PIN_LORA_MISO, PIN_LORA_MOSI);
    LoRa.setPins(PIN_LORA_NSS, PIN_LORA_RESET, PIN_LORA_DIO0);
    if (!LoRa.begin(loraFreq))
    {
        Serial.println("fail to initial LoRa");
        if (_oled)
        {
            _oled->setCursor(0, 10);
            _oled->println("LoRa Init Fail");
            _oled->display();
            vTaskDelay(1000);
            _oled->clearDisplay();
        }
        // return false;
    }
    else
    {
        Serial.println("LoRa initialized successfully.");
        if (_oled)
        {
            _oled->setCursor(0, 10);
            _oled->println("LoRa OK");
            _oled->display();
            vTaskDelay(1000); // Delay to show the message
            _oled->clearDisplay();
        }
    }

    // Step 5: Initialize ADS1115 ADC
    Serial.println("Initializing ADC...");
    if (_oled)
    {
        _oled->clearDisplay();
        _oled->setCursor(0, 0);
        _oled->println("Init ADC...");
        _oled->display();
        vTaskDelay(300); // Delay to show the message
    }
    bool adcInit = initADC(ADS1115_ADDRESS);
    if (!adcInit)
    {
        Serial.println("fail to initialize ADS1115");
        if (_oled)
        {
            _oled->setCursor(0, 10);
            _oled->println("ADS Init Fail");
            _oled->display();
            vTaskDelay(1000);
            _oled->clearDisplay();
        }
        return false;
    }
    else
    {
        Serial.println("ADS1115 initialized successfully.");
        if (_oled)
        {
            _oled->setCursor(0, 10);
            _oled->println("ADS OK");
            _oled->display();
            vTaskDelay(1000); // Delay to show the message
            _oled->clearDisplay();
        }
    }

    // Final status messages
    Serial.println("I2C initialized successfully.");
    Serial.println("LoRa initialized successfully.");
    Serial.println("Tenergy32Hub initialized successfully.");
    if (_oled)
    {
        _oled->clearDisplay();
        _oled->setCursor(0, 0);
        _oled->println("Initialization");
        _oled->setCursor(0, 10);
        _oled->println("Complete");
        _oled->display();
        vTaskDelay(1000);
    }

    // Show library version on OLED if available
    showLibraryVersion(); // Show library version on serial monitor
    vTaskDelay(1000);     // Delay to show the message

    beep(2, 100); // Beep twice to indicate successful initialization
    return true;
}

/***********************************************************************
 * FUNCTION:    readSlideSwitch
 * DESCRIPTION: Reads the state of the slide switch.
 * PARAMETERS:  none
 * RETURNED:    true if the switch is on, false otherwise.
 ***********************************************************************/
bool Tenergy32Hub::readSlideSwitch() { return digitalRead(PIN_SLIDE_SWITCH) == HIGH; }

/***********************************************************************
 * FUNCTION:    readSW1
 * DESCRIPTION: Reads the state of switch SW1.
 * PARAMETERS:  none
 * RETURNED:    true if the switch is pressed, false otherwise.
 ***********************************************************************/
bool Tenergy32Hub::readSW1() { return digitalRead(PIN_SW1) == LOW; }

/***********************************************************************
 * FUNCTION:    readSW2
 * DESCRIPTION: Reads the state of switch SW2.
 * PARAMETERS:  none
 * RETURNED:    true if the switch is pressed, false otherwise.
 ***********************************************************************/
bool Tenergy32Hub::readSW2() { return digitalRead(PIN_SW2) == LOW; }

/***********************************************************************
 * FUNCTION:    readMotionSensor
 * DESCRIPTION: Reads the state of the motion sensor.
 * PARAMETERS:  none
 * RETURNED:    1 if motion is detected, 0 otherwise.
 ***********************************************************************/
int Tenergy32Hub::readMotionSensor() { return digitalRead(PIN_MOTION_SENSOR); }

/***********************************************************************
 * FUNCTION:    readWaterLeak
 * DESCRIPTION: Reads the state of the water leak sensor.
 * PARAMETERS:  none
 * RETURNED:    1 if water is detected, 0 otherwise.
 ***********************************************************************/
int Tenergy32Hub::readWaterLeak() { return digitalRead(PIN_WATER_LEAK); }

/***********************************************************************
 * FUNCTION:    relayOn
 * DESCRIPTION: Turns the relay on.
 * PARAMETERS:  none
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::relayOn() { digitalWrite(PIN_RELAY, HIGH); }

/***********************************************************************
 * FUNCTION:    relayOff
 * DESCRIPTION: Turns the relay off.
 * PARAMETERS:  none
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::relayOff() { digitalWrite(PIN_RELAY, LOW); }

/***********************************************************************
 * FUNCTION:    setRelay
 * DESCRIPTION: Turns the relay off.
 * PARAMETERS:  none
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::setRelay(bool state)
{
    if (state)
        relayOn();
    else
        relayOff();
}

/***********************************************************************
 * FUNCTION:    readRelayState
 * DESCRIPTION: Reads the state of the relay.
 * PARAMETERS:  none
 * RETURNED:    true if the relay is on, false otherwise.
 ***********************************************************************/
bool Tenergy32Hub::readRelayState()
{
    // Assumes digitalWrite(PIN_RELAY, HIGH) means "on"
    return (digitalRead(PIN_RELAY) == HIGH);
}

/***********************************************************************
 * FUNCTION:    setBlueLED
 * DESCRIPTION: Controls the state of the blue LED.
 * PARAMETERS:  on - true to turn on, false to turn off.
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::setBlueLED(bool on) { digitalWrite(PIN_LED_BLUE, on ? HIGH : LOW); }

/***********************************************************************
 * FUNCTION:    setRedLED
 * DESCRIPTION: Controls the state of the red LED.
 * PARAMETERS:  on - true to turn on, false to turn off.
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::setRedLED(bool on) { digitalWrite(PIN_LED_RED, on ? HIGH : LOW); }

/***********************************************************************
 * FUNCTION:    setbuildingLED
 * DESCRIPTION: Controls the state of the building LED.
 * PARAMETERS:  on - true to turn on, false to turn off.
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::setbuildingLED(bool on)
{
    // Configure PIN2 as output (if not already done)
    pinMode(2, OUTPUT);
    digitalWrite(2, on ? HIGH : LOW);
}
/***********************************************************************
 * FUNCTION:    beep
 * DESCRIPTION: Activates the buzzer for a specified duration.
 * PARAMETERS:  ms - duration in milliseconds.
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::beep(uint8_t times, uint16_t ms)
{
    const uint8_t channel = 0;       // LEDC channel to use
    const uint32_t frequency = 2048; // Frequency in Hz matching the buzzer's resonant frequency
    const uint8_t resolution = 8;    // LEDC resolution

    // Initialize LEDC channel for the buzzer
    ledcSetup(channel, frequency, resolution);
    ledcAttachPin(PIN_BUZZER, channel);

    for (uint8_t i = 0; i < times; i++)
    {
        // Start the tone
        ledcWriteTone(channel, frequency);
        delay(ms); // Wait for the specified duration
        // Stop the tone
        ledcWriteTone(channel, 0);
        delay(50); // Brief pause between beeps
    }
}

/***********************************************************************
 * FUNCTION:    resetCharger
 * DESCRIPTION: Resets the charger by toggling the reset pin.
 * PARAMETERS:  none
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::resetCharger()
{
    digitalWrite(PIN_CHARGER_RESET, LOW);
    delay(10);
    digitalWrite(PIN_CHARGER_RESET, HIGH);
}

/***********************************************************************
 * FUNCTION:    sendLoRa
 * DESCRIPTION: Sends data over LoRa communication.
 * PARAMETERS:  data - pointer to the data to send.
 *              len - length of the data.
 * RETURNED:    true if the packet was sent successfully, false otherwise.
 ***********************************************************************/
bool Tenergy32Hub::sendLoRa(const uint8_t *data, size_t len)
{
    LoRa.beginPacket();
    LoRa.write(data, len);
    return LoRa.endPacket() == 1;
}

/***********************************************************************
 * FUNCTION:    receiveLoRa
 * DESCRIPTION: Receives data over LoRa communication.
 * PARAMETERS:  buffer - pointer to the buffer to store received data.
 *              maxLen - maximum length of the buffer.
 *              received - reference to store the number of bytes received.
 * RETURNED:    true if data was received, false otherwise.
 ***********************************************************************/
bool Tenergy32Hub::receiveLoRa(uint8_t *buffer, size_t maxLen, int &received)
{
    int packetSize = LoRa.parsePacket();
    if (!packetSize)
        return false;
    received = LoRa.readBytes(buffer, min((int)maxLen, packetSize));
    return true;
}

/***********************************************************************
 * FUNCTION:    rs485TransmitMode
 * DESCRIPTION: Sets the RS485 module to transmit mode.
 * PARAMETERS:  en - true to enable transmit mode, false to disable.
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::rs485TransmitMode(bool en)
{
    // TODO: tie DE/RE pins if available
}

/***********************************************************************
 * FUNCTION:    sendRS485
 * DESCRIPTION: Sends data over RS485 communication.
 * PARAMETERS:  data - pointer to the data to send.
 *              len - length of the data.
 * RETURNED:    number of bytes sent.
 ***********************************************************************/
size_t Tenergy32Hub::sendRS485(const uint8_t *data, size_t len)
{
    Serial2.begin(9600, SERIAL_8N1, PIN_RX_485, PIN_TX_485);
    return Serial2.write(data, len);
}

/***********************************************************************
 * FUNCTION:    receiveRS485
 * DESCRIPTION: Receives data over RS485 communication.
 * PARAMETERS:  buffer - pointer to the buffer to store received data.
 *              maxLen - maximum length of the buffer.
 * RETURNED:    number of bytes received.
 ***********************************************************************/
size_t Tenergy32Hub::receiveRS485(uint8_t *buffer, size_t maxLen)
{
    Serial2.begin(9600, SERIAL_8N1, PIN_RX_485, PIN_TX_485);
    return Serial2.readBytes(buffer, maxLen);
}

/***********************************************************************
 * FUNCTION:    initI2C
 * DESCRIPTION: Initializes the I2C communication.
 * PARAMETERS:  none
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::initI2C()
{
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
}

/***********************************************************************
 * FUNCTION:    initOLED
 * DESCRIPTION: Initializes the OLED display.
 * PARAMETERS:  addr - I2C address of the OLED display.
 * RETURNED:    true if initialization is successful, false otherwise.
 ***********************************************************************/
bool Tenergy32Hub::initOLED(uint8_t addr)
{
    _oled = new Adafruit_SSD1306(128, 32, &Wire);
    return _oled->begin(SSD1306_SWITCHCAPVCC, addr);
}

/***********************************************************************
 * FUNCTION:    displayOLED
 * DESCRIPTION: Displays text on the OLED screen.
 * PARAMETERS:  text - pointer to the text to display.
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::displayOLED(const char *text)
{
    if (!_oled)
        return;
    _oled->clearDisplay();
    _oled->setTextSize(1);
    _oled->setTextColor(SSD1306_WHITE);
    _oled->setCursor(0, 0);
    _oled->println(text);
    _oled->display();
}

/***********************************************************************
 * FUNCTION:    displayOLEDInfo
 * DESCRIPTION: Displays information on the OLED screen.
 * PARAMETERS:  none
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::displayOLEDInfo()
{
    if (!_oled)
        return;

    _oled->clearDisplay();              // Clear any previous content
    _oled->setTextSize(1);              // Set text size (adjust as needed)
    _oled->setTextColor(SSD1306_WHITE); // Set text color

    // First line
    const char *line1 = "Tenergy Innovation";
    int16_t x1, y1;
    uint16_t w, h;
    _oled->getTextBounds(line1, 0, 0, &x1, &y1, &w, &h);
    int xpos1 = (128 - w) / 2; // Center horizontally for a 128px wide display
    _oled->setCursor(xpos1, 0);
    _oled->println(line1);

    // Second line
    const char *line2 = "Tenergy32Hub IoT";
    _oled->getTextBounds(line2, 0, 0, &x1, &y1, &w, &h);
    int xpos2 = (128 - w) / 2;
    _oled->setCursor(xpos2, 10); // Adjust vertical position as needed
    _oled->println(line2);

    _oled->display();
}

/***********************************************************************
 * FUNCTION:    initLCD
 * DESCRIPTION: Initializes the LCD display.
 * PARAMETERS:  addr - I2C address of the LCD display.
 * RETURNED:    true if initialization is successful, false otherwise.
 ***********************************************************************/
bool Tenergy32Hub::initLCD(uint8_t addr, uint8_t cols, uint8_t rows)
{
    Serial.println("Initializing LCD...");
    if (_oled)
    {
        _oled->clearDisplay();
        _oled->setCursor(0, 0);
        _oled->println("Init LCD...");
        _oled->display();
        vTaskDelay(300); // Delay to show the message
    }
    _lcd = new LiquidCrystal_I2C(addr, cols, rows);
    _lcd->init();
    _lcd->backlight();
    return true;
}

/***********************************************************************
 * FUNCTION:    displayLCD
 * DESCRIPTION: Displays text on the LCD screen at a specific position.
 * PARAMETERS:  text - pointer to the text to display.
 *              col - column position.
 *              row - row position.
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::displayLCD(const char *text, uint8_t col, uint8_t row)
{
    if (!_lcd)
        return;
    _lcd->setCursor(col, row);
    _lcd->print(text);
}

/***********************************************************************
 * FUNCTION:    clearLCD
 * DESCRIPTION: Clears the LCD display and resets the cursor position.
 * PARAMETERS:  none
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::clearLCD()
{
    if (_lcd)
    {
        _lcd->clear();
        _lcd->setCursor(0, 0); // Reset cursor position to the top-left corner
    }
}

/***********************************************************************
 * FUNCTION:    getInstance
 * DESCRIPTION: Returns the singleton instance of the Tenergy32Hub class.
 * PARAMETERS:  none
 * RETURNED:    pointer to the Tenergy32Hub instance.
 ***********************************************************************/
void Tenergy32Hub::onBacklightLCD()
{
    if (_lcd)
    {
        _lcd->backlight(); // Turn on the backlight
    }
}

/***********************************************************************
 * FUNCTION:    offBacklightLCD
 * DESCRIPTION: Turns off the backlight of the LCD display.
 * PARAMETERS:  none
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::offBacklightLCD()
{
    if (_lcd)
    {
        _lcd->noBacklight(); // Turn off the backlight
    }
}

/***********************************************************************
 * FUNCTION:    initADC
 * DESCRIPTION: Initializes the ADC module.
 * PARAMETERS:  addr - I2C address of the ADC module.
 * RETURNED:    true if initialization is successful, false otherwise.
 ***********************************************************************/
bool Tenergy32Hub::initADC(uint8_t addr)
{
    _ads = new Adafruit_ADS1115(); // Use default constructor
    return _ads->begin();
}

/***********************************************************************
 * FUNCTION:    readADCChannel
 * DESCRIPTION: Reads a specific channel of the ADC module.
 * PARAMETERS:  chan - channel number (0-3).
 * RETURNED:    ADC value of the specified channel.
 ***********************************************************************/
int16_t Tenergy32Hub::readADCChannel(uint8_t chan)
{
    if (!_ads)
        return 0;
    switch (chan)
    {
    case 0:
        return _ads->readADC_SingleEnded(0);
    case 1:
        return _ads->readADC_SingleEnded(1);
    case 2:
        return _ads->readADC_SingleEnded(2);
    case 3:
        return _ads->readADC_SingleEnded(3);
    default:
        return 0;
    }
}

/***********************************************************************
 * FUNCTION:    readPotentiometer
 * DESCRIPTION: Reads the analog value from the potentiometer connected
 *              to the ADS1115 channel AIN2.
 * PARAMETERS:  none
 * RETURNED:    The ADC reading as a signed 16-bit integer.
 ***********************************************************************/
int16_t Tenergy32Hub::readPotentiometer()
{
    // Channel 2 corresponds to AIN2 on the ADS1115

    return readADCChannel(2);
}

/***********************************************************************
 * FUNCTION:    displayOLEDLines
 * DESCRIPTION: Displays text on up to 5 separate lines on the OLED.
 * PARAMETERS:  line1 - pointer to the first line of text.
 *              line2 - pointer to the second line of text.
 *              line3 - pointer to the third line of text.
 *              line4 - pointer to the fourth line of text.
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::displayOLEDLines(const char *line1, const char *line2, const char *line3, const char *line4)
{
    if (!_oled)
        return;

    _oled->clearDisplay();
    _oled->setTextSize(1);
    _oled->setTextColor(SSD1306_WHITE);

    // กำหนดความสูงของหน้าจอ (32 พิกเซล)
    const int displayHeight = 32;

    // อาเรย์ของบรรทัดที่เป็นไปได้ 4 บรรทัด
    const char *lines[4] = {line1, line2, line3, line4};

    // นับจำนวนบรรทัดที่มีข้อความ (non-empty)
    int count = 0;
    for (int i = 0; i < 4; i++)
    {
        if (lines[i] && strlen(lines[i]) > 0)
        {
            count++;
        }
    }
    if (count == 0)
        return;

    // กำหนดค่าเริ่มต้นของความสูงบรรทัด และเพิ่มระยะห่างเพิ่มเติม
    int baseLineHeight = 8; // ค่าเริ่มต้นสำหรับ textSize(1)
    int extraSpacing = 8;   // เพิ่มระยะห่าง 8 พิกเซล
    int lineHeight = baseLineHeight + extraSpacing;

    // ตรวจสอบว่าจำนวนบรรทัดที่แสดงรวมกันเกินความสูงของหน้าจอหรือไม่
    if (count * lineHeight > displayHeight)
    {
        // ปรับความสูงบรรทัดให้พอดีหน้าจอ (อาจจะเล็กลง)
        lineHeight = displayHeight / count;
    }

    // คำนวณระยะ offset เพื่อจัดให้อยู่ตรงกลางในแนวตั้ง
    int offset = (displayHeight - (count * lineHeight)) / 2;

    // แสดงแต่ละบรรทัดที่มีข้อความ
    int printed = 0;
    for (int i = 0; i < 4; i++)
    {
        if (lines[i] && strlen(lines[i]) > 0)
        {
            _oled->setCursor(0, offset + printed * lineHeight);
            _oled->println(lines[i]);
            printed++;
        }
    }

    _oled->display();
}

/***********************************************************************
 * FUNCTION:    clearOLED
 * DESCRIPTION: Clears the OLED display and resets the cursor position.
 * PARAMETERS:  none
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::clearOLED()
{
    if (_oled)
    {
        _oled->clearDisplay();
        _oled->setCursor(0, 0);
        _oled->display();
    }
}
/***********************************************************************
 * FUNCTION:    marioSound
 * DESCRIPTION: Plays a simple melody using the buzzer.
 * PARAMETERS:  none
 * RETURNED:    none
 ***********************************************************************/
// First, define note frequency constants if not defined already.
#define NOTE_E7 2637
#define NOTE_C7 2093
#define NOTE_G7 3135
#define NOTE_G6 1568
void Tenergy32Hub::marioSound()
{
    // A very simplified version of the Mario theme melody:
    // Notes array (0 indicates a rest)
    int melody[] = {NOTE_E7, NOTE_E7, 0, NOTE_E7,
                    0, NOTE_C7, NOTE_E7, 0,
                    NOTE_G7, 0, 0, 0,
                    NOTE_G6, 0, 0, 0};
    // Corresponding note durations (in milliseconds)
    int noteDurations[] = {150, 150, 150, 150,
                           150, 150, 150, 150,
                           150, 150, 150, 150,
                           150, 150, 150, 150};
    const int numNotes = sizeof(melody) / sizeof(melody[0]);

    // Use LEDC channel 0 to play the melody.
    const uint8_t channel = 0;
    const uint8_t resolution = 8;
    // Setup channel; the initial frequency value will be overwritten by each note.
    ledcSetup(channel, 2000, resolution);
    ledcAttachPin(PIN_BUZZER, channel);

    for (int i = 0; i < numNotes; i++)
    {
        int noteDuration = noteDurations[i];
        if (melody[i] == 0)
        {
            // Rest note - no sound
            ledcWriteTone(channel, 0);
            delay(noteDuration);
        }
        else
        {
            // Play the note at the given frequency for noteDuration ms.
            ledcWriteTone(channel, melody[i]);
            delay(noteDuration);
            // Stop the note
            ledcWriteTone(channel, 0);
            // Brief pause between notes
            delay(50);
        }
    }
}

/***********************************************************************
 * FUNCTION:    angryBirdSound
 * DESCRIPTION: Plays a simple melody using the buzzer.
 * PARAMETERS:  none
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::angryBirdSound()
{
    // Extended "Angry Bird" sound sequence.
    // Frequencies for the melody (0 indicates a rest)
    int melody[] = {
        2200, 2100, 2000, 1900, 1800, 1700,
        1600, 1700, 1800, 1900, 2000, 2100,
        2200, 2200, 0, 2200,
        2100, 2100, 0, 2100,
        2000, 0, 2000, 2100};

    // Corresponding note durations in milliseconds
    int noteDurations[] = {
        150, 150, 150, 150, 150, 150,
        150, 150, 150, 150, 150, 150,
        200, 200, 100, 200,
        150, 150, 100, 150,
        200, 100, 200, 150};

    const int numNotes = sizeof(melody) / sizeof(melody[0]);

    // Use LEDC channel 0 to play the sound.
    const uint8_t channel = 0;
    const uint8_t resolution = 8;
    ledcSetup(channel, 2000, resolution);
    ledcAttachPin(PIN_BUZZER, channel);

    for (int i = 0; i < numNotes; i++)
    {
        int duration = noteDurations[i];
        if (melody[i] == 0)
        {
            // Rest note: no sound.
            ledcWriteTone(channel, 0);
            delay(duration);
        }
        else
        {
            // Play the note.
            ledcWriteTone(channel, melody[i]);
            delay(duration);
            // Turn off the note.
            ledcWriteTone(channel, 0);
            // Brief pause between notes.
            delay(50);
        }
    }
}

/***********************************************************************
 * FUNCTION:    blinkRedLED
 * DESCRIPTION: Starts blinking the red LED at a specified interval.
 * PARAMETERS:  intervalMillis - duration of the full blink cycle in milliseconds.
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::redLEDToggle()
{
    _instance->_redState = !_instance->_redState;
    _instance->setRedLED(_instance->_redState);
}

/***********************************************************************
 * FUNCTION:    blinkBlueLED
 * DESCRIPTION: Starts blinking the blue LED at a specified interval.
 * PARAMETERS:  intervalMillis - duration of the full blink cycle in milliseconds.
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::blueLEDToggle()
{
    _instance->_blueState = !_instance->_blueState;
    _instance->setBlueLED(_instance->_blueState);
}

/***********************************************************************
 * FUNCTION:    blinkbuildingLED
 * DESCRIPTION: Starts blinking the building LED at a specified interval.
 * PARAMETERS:  intervalMillis - duration of the full blink cycle in milliseconds.
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::buildingLEDToggle()
{
    _instance->_buildingState = !_instance->_buildingState;
    _instance->setbuildingLED(_instance->_buildingState);
}

// -------------------------------------------------------------------
// Blink functions using Ticker callbacks

/***********************************************************************
 * FUNCTION:    blinkRedLED
 * DESCRIPTION: Starts blinking the red LED at a specified interval.
 * PARAMETERS:  intervalMillis - duration of the full blink cycle in milliseconds.
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::blinkRedLED(uint32_t intervalMillis)
{
    // Detach any existing callback.
    _tickerRed.detach();
    if (intervalMillis == 0)
    {
        setRedLED(false);
        _redState = false;
        return;
    }
    // Use half period for toggling ON/OFF.
    _tickerRed.attach_ms(intervalMillis / 2, redLEDToggle);
}

/***********************************************************************
 * FUNCTION:    blinkBlueLED
 * DESCRIPTION: Starts blinking the blue LED at a specified interval.
 * PARAMETERS:  intervalMillis - duration of the full blink cycle in milliseconds.
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::blinkBlueLED(uint32_t intervalMillis)
{
    _tickerBlue.detach();
    if (intervalMillis == 0)
    {
        setBlueLED(false);
        _blueState = false;
        return;
    }
    _tickerBlue.attach_ms(intervalMillis / 2, blueLEDToggle);
}

/***********************************************************************
 * FUNCTION:    blinkbuildingLED
 * DESCRIPTION: Starts blinking the building LED at a specified interval.
 * PARAMETERS:  intervalMillis - duration of the full blink cycle in milliseconds.
 * RETURNED:    none
 ***********************************************************************/
void Tenergy32Hub::blinkbuildingLED(uint32_t intervalMillis)
{
    _tickerBuilding.detach();
    if (intervalMillis == 0)
    {
        setbuildingLED(false);
        _buildingState = false;
        return;
    }
    _tickerBuilding.attach_ms(intervalMillis / 2, buildingLEDToggle);
}

/***********************************************************************
 * FUNCTION:    readBattery_SOC
 * DESCRIPTION: Reads the battery percentage (0-100%).
 * PARAMETERS:  batteryVoltage - reference to store the battery voltage.
 *              soc - reference to store the state of charge (SOC) percentage.
 * RETURNED:    True if the battery is charging (voltage > 4.10V), false otherwise.
 * CONDITIONS:  - Tenergy32Hub must be install HM-CD42 charger module at back side of the board.
 *              - Must to remove R13 0Ω resistor from the board.
 ***********************************************************************/
bool Tenergy32Hub::readBattery_SOC(float &batteryVoltage, float &soc)
{
    batteryVoltage = (readADCChannel(3) * 0.1875 / 1000.0) * (2.0833); // แปลงค่า ADC เป็นแรงดัน
    char _line1[32];
    char _line2[32];
    // พารามิเตอร์ที่ได้จากการ fitting
    const float _a = -274.76200211f;
    const float _b = 3192.30123651f;
    const float _c = -12163.64160305f;
    const float _d = 15250.08928304f;

    // คำนวณสมการพหุนามดีกรี 3
    soc = _a * powf(batteryVoltage, 3) + _b * powf(batteryVoltage, 2) + _c * batteryVoltage + _d;
    // Clamp ให้อยู่ในช่วง [0, 100]
    if (soc < 0.0f)
        soc = 0.0f;
    if (soc > 100.0f)
        soc = 100.0f;

    if (batteryVoltage > 4.10f)
    {
        Serial.printf("Charging battery voltage %.2f V\r\n", batteryVoltage);
        snprintf(_line1, sizeof(_line1), "Charging batt: %.2f V", batteryVoltage);
        displayOLEDLines(_line1, "", "", "");
        return true;
    }
    else
    {
        Serial.printf("Voltage: %.2f V, Estimated SOC: %.2f %%\r\n", batteryVoltage, soc);
        snprintf(_line1, sizeof(_line1), "Battery: %.2f V", batteryVoltage);
        snprintf(_line2, sizeof(_line2), "Estimated SOC: %.1f%%", soc);
        displayOLEDLines(_line1, _line2, "", "");
        return false;
    }
}

/***********************************************************************
 * FUNCTION:    readSDM120Float
 * DESCRIPTION: อ่านค่า float (32-bit) จากรีจิสเตอร์ของ SDM120 Modbus RTU
 * PARAMETERS:  slaveAddr - Modbus slave address (ปกติ SDM120 = 1)
 *              regAddr   - Register address (เช่น 0x0000 สำหรับ Voltage)
 *              value     - reference สำหรับเก็บค่าที่อ่านได้
 *              serial    - Serial port ที่ใช้ (เช่น Serial2)
 *              baud      - baudrate (default 9600)
 * RETURNED:    true ถ้าอ่านสำเร็จ, false ถ้าอ่านไม่สำเร็จ
 ***********************************************************************/
bool Tenergy32Hub::readSDM120Float(uint8_t slaveAddr, uint16_t regAddr, float &value, HardwareSerial &serial, uint32_t baud)
{
    // SDM120 ใช้ Modbus RTU Function 0x04 (Read Input Registers)
    // ส่งคำสั่ง: [slaveAddr][0x04][regHi][regLo][0x00][0x02][CRC_L][CRC_H]
    uint8_t frame[8];
    frame[0] = slaveAddr;
    frame[1] = 0x04;
    frame[2] = (regAddr >> 8) & 0xFF;
    frame[3] = regAddr & 0xFF;
    frame[4] = 0x00;
    frame[5] = 0x02; // อ่าน 2 รีจิสเตอร์ (4 bytes = float)
    // คำนวณ CRC16
    uint16_t crc = 0xFFFF;
    for (int i = 0; i < 6; i++) {
        crc ^= frame[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc = crc >> 1;
        }
    }
    frame[6] = crc & 0xFF;         // CRC Low byte
    frame[7] = (crc >> 8) & 0xFF;  // CRC High byte

    serial.begin(baud, SERIAL_8N1, PIN_RX_485, PIN_TX_485);
    while (serial.available()) serial.read(); // flush

    serial.write(frame, 8);
    serial.flush();

    // รอรับข้อมูลตอบกลับ (9 bytes)
    uint32_t start = millis();
    while (serial.available() < 9 && (millis() - start) < 200) {
        delay(1);
    }
    if (serial.available() < 9) return false;

    uint8_t resp[9];
    for (int i = 0; i < 9; i++) resp[i] = serial.read();

    // ตรวจสอบ slaveAddr, function, byte count
    if (resp[0] != slaveAddr || resp[1] != 0x04 || resp[2] != 4) return false;

    // ตรวจสอบ CRC
    uint16_t crc_resp = 0xFFFF;
    for (int i = 0; i < 7; i++) {
        crc_resp ^= resp[i];
        for (int j = 0; j < 8; j++) {
            if (crc_resp & 0x0001)
                crc_resp = (crc_resp >> 1) ^ 0xA001;
            else
                crc_resp = crc_resp >> 1;
        }
    }
    uint16_t crc_recv = resp[7] | (resp[8] << 8);
    if (crc_resp != crc_recv) return false;

    // แก้ไขการแปลง byte order ให้ถูกต้อง (SDM120 ส่ง [HiHi][HiLo][LoHi][LoLo])
    uint8_t floatBytes[4];
    floatBytes[0] = resp[6]; // LoLo
    floatBytes[1] = resp[5]; // LoHi
    floatBytes[2] = resp[4]; // HiLo
    floatBytes[3] = resp[3]; // HiHi
    float f;
    memcpy(&f, floatBytes, 4);
    value = f;
    return true;
}

/***********************************************************************
 * FUNCTION:    readSDM120All
 * DESCRIPTION: อ่านค่าหลักๆ จาก SDM120 (Voltage, Current, Active Power, Import Energy)
 * PARAMETERS:  slaveAddr - Modbus slave address
 *              voltage, current, activePower, importEnergy - reference สำหรับเก็บค่าที่อ่านได้
 *              serial    - Serial port ที่ใช้ (เช่น Serial2)
 *              baud      - baudrate (default 9600)
 * RETURNED:    true ถ้าอ่านสำเร็จทั้งหมด, false ถ้ามีค่าใดอ่านไม่สำเร็จ
 ***********************************************************************/
bool Tenergy32Hub::readSDM120All(uint8_t slaveAddr, float &voltage, float &current, float &activePower, float &importEnergy, HardwareSerial &serial, uint32_t baud)
{
    bool ok = true;
    ok &= readSDM120Float(slaveAddr, SDM120_REG_VOLTAGE, voltage, serial, baud);
    ok &= readSDM120Float(slaveAddr, SDM120_REG_CURRENT, current, serial, baud);
    ok &= readSDM120Float(slaveAddr, SDM120_REG_ACTIVE_POWER, activePower, serial, baud);
    ok &= readSDM120Float(slaveAddr, SDM120_REG_IMPORT_ENERGY, importEnergy, serial, baud);
    return ok;
}

/***********************************************************************
 * FUNCTION:    readSDM120All
 * DESCRIPTION: อ่านค่าหลักๆ จาก SDM120 (Voltage, Current, Active Power, Import Energy, Power Factor, Frequency)
 * PARAMETERS:  slaveAddr - Modbus slave address
 *              voltage, current, activePower, importEnergy, powerFactor, frequency - reference สำหรับเก็บค่าที่อ่านได้
 *              serial    - Serial port ที่ใช้ (เช่น Serial2)
 *              baud      - baudrate (default 9600)
 * RETURNED:    true ถ้าอ่านสำเร็จทั้งหมด, false ถ้ามีค่าใดอ่านไม่สำเร็จ
 ***********************************************************************/
bool Tenergy32Hub::readSDM120All(
    uint8_t slaveAddr,
    float &voltage, float &current, float &activePower, float &importEnergy,
    float &powerFactor, float &frequency,
    HardwareSerial &serial, uint32_t baud)
{
    bool ok = true;
    ok &= readSDM120Float(slaveAddr, SDM120_REG_VOLTAGE, voltage, serial, baud);
    ok &= readSDM120Float(slaveAddr, SDM120_REG_CURRENT, current, serial, baud);
    ok &= readSDM120Float(slaveAddr, SDM120_REG_ACTIVE_POWER, activePower, serial, baud);
    ok &= readSDM120Float(slaveAddr, SDM120_REG_IMPORT_ENERGY, importEnergy, serial, baud);
    ok &= readSDM120Float(slaveAddr, SDM120_REG_POWER_FACTOR, powerFactor, serial, baud);
    ok &= readSDM120Float(slaveAddr, SDM120_REG_FREQUENCY, frequency, serial, baud);
    return ok;
}

/***********************************************************************
 * FUNCTION:    readSDM120Voltage
 * DESCRIPTION: Reads the voltage from SDM120.
 * PARAMETERS:  slaveAddr - Modbus slave address
 *              voltage - reference to store the voltage value
 *              serial - Serial port to use (default Serial2)
 *              baud - baudrate (default 9600)
 * RETURNED:    true if reading is successful, false otherwise
 ***********************************************************************/
bool Tenergy32Hub::readSDM120Voltage(uint8_t slaveAddr, float &voltage, HardwareSerial &serial, uint32_t baud)
{
    return readSDM120Float(slaveAddr, SDM120_REG_VOLTAGE, voltage, serial, baud);
}
/***********************************************************************
 * FUNCTION:    readSDM120Current
 * DESCRIPTION: Reads the current from SDM120.
 * PARAMETERS:  slaveAddr - Modbus slave address
 *              current - reference to store the current value
 *              serial - Serial port to use (default Serial2)
 *              baud - baudrate (default 9600)
 * RETURNED:    true if reading is successful, false otherwise
 ***********************************************************************/
bool Tenergy32Hub::readSDM120Current(uint8_t slaveAddr, float &current, HardwareSerial &serial, uint32_t baud)
{
    return readSDM120Float(slaveAddr, SDM120_REG_CURRENT, current, serial, baud);
}
bool Tenergy32Hub::readSDM120ActivePower(uint8_t slaveAddr, float &activePower, HardwareSerial &serial, uint32_t baud)
{
    return readSDM120Float(slaveAddr, SDM120_REG_ACTIVE_POWER, activePower, serial, baud);
}
bool Tenergy32Hub::readSDM120ImportEnergy(uint8_t slaveAddr, float &importEnergy, HardwareSerial &serial, uint32_t baud)
{
    return readSDM120Float(slaveAddr, SDM120_REG_IMPORT_ENERGY, importEnergy, serial, baud);
}
bool Tenergy32Hub::readSDM120PowerFactor(uint8_t slaveAddr, float &powerFactor, HardwareSerial &serial, uint32_t baud)
{
    return readSDM120Float(slaveAddr, SDM120_REG_POWER_FACTOR, powerFactor, serial, baud);
}
bool Tenergy32Hub::readSDM120Frequency(uint8_t slaveAddr, float &frequency, HardwareSerial &serial, uint32_t baud)
{
    return readSDM120Float(slaveAddr, SDM120_REG_FREQUENCY, frequency, serial, baud);
}