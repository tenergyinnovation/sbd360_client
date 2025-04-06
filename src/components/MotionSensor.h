#ifndef MOTIONSENSOR_H
#define MOTIONSENSOR_H

class MotionSensor {
public:
    MotionSensor(int pin);
    void begin();
    bool isMotionDetected();

private:
    int _pin;
};

#endif // MOTIONSENSOR_H