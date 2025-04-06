#ifndef RELAY_H
#define RELAY_H

class Relay {
public:
    Relay(int pin);
    void turnOn();
    void turnOff();
    bool isOn();

private:
    int relayPin;
    bool state;
};

#endif // RELAY_H