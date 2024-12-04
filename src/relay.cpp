#include <Arduino.h>
#include "relay.h"

int relayPin;
int relayPin2;

void initRelay(int pin)
{
    relayPin = pin;
    pinMode(relayPin, OUTPUT);
}

void initRelay2(int pin)
{
    relayPin2 = pin;
    pinMode(relayPin2, OUTPUT);
}

void controlRelay(bool state)
{
    digitalWrite(relayPin, state ? HIGH : LOW);
}

void controlRelay2(bool state)
{
    digitalWrite(relayPin2, state ? LOW : HIGH);
}
