#ifndef WATERLEAKSENSOR_H
#define WATERLEAKSENSOR_H

class WaterLeakSensor {
public:
    WaterLeakSensor(int pin);
    void begin();
    bool isLeakDetected();

private:
    int _pin;
};

#endif // WATERLEAKSENSOR_H