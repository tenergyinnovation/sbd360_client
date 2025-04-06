#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

class Potentiometer {
public:
    Potentiometer(int pin);
    int readValue();

private:
    int _pin;
};

#endif // POTENTIOMETER_H