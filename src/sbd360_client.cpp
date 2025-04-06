#include "sbd360_client.h"
#include "OLEDDisplay.h"
#include "LoRaModule.h"
#include "Relay.h"
#include "RS485.h"
#include "Potentiometer.h"
#include "MotionSensor.h"
#include "WaterLeakSensor.h"

SBD360Client::SBD360Client() {
    // Initialize components
    oledDisplay = new OLEDDisplay();
    loraModule = new LoRaModule();
    relay = new Relay();
    rs485 = new RS485();
    potentiometer = new Potentiometer();
    motionSensor = new MotionSensor();
    waterLeakSensor = new WaterLeakSensor();
}

void SBD360Client::begin() {
    // Initialize all components
    oledDisplay->init();
    loraModule->begin();
    relay->turnOff(); // Ensure relay is off initially
    rs485->begin();
}

void SBD360Client::update() {
    // Update sensor readings and display data
    if (motionSensor->isMotionDetected()) {
        oledDisplay->displayText("Motion Detected!");
    } else {
        oledDisplay->clearDisplay();
    }

    if (waterLeakSensor->isLeakDetected()) {
        oledDisplay->displayText("Water Leak Detected!");
    }

    // Read potentiometer value
    int potValue = potentiometer->readValue();
    // Process potValue as needed
}

void SBD360Client::getSensorData() {
    // Retrieve and process data from sensors
    // This can be expanded based on specific requirements
}