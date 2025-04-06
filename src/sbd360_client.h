#ifndef SBD360_CLIENT_H
#define SBD360_CLIENT_H

#include <Arduino.h>
#include "OLEDDisplay.h"
#include "LoRaModule.h"
#include "Relay.h"
#include "RS485.h"
#include "Potentiometer.h"
#include "MotionSensor.h"
#include "WaterLeakSensor.h"

class SBD360Client {
public:
    SBD360Client();

    void begin();
    void update();
    String getSensorData();

private:
    OLEDDisplay oledDisplay;
    LoRaModule loraModule;
    Relay relay;
    RS485 rs485;
    Potentiometer potentiometer;
    MotionSensor motionSensor;
    WaterLeakSensor waterLeakSensor;

    void initializeComponents();
};

#endif // SBD360_CLIENT_H