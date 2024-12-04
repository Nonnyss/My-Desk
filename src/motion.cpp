#include <Arduino.h>
#include "motion.h"

int motionSensorPin;

void initMotionSensor(int pin)
{
    motionSensorPin = pin;
    pinMode(motionSensorPin, INPUT);
}

bool readMotion()
{
    return digitalRead(motionSensorPin) == HIGH;
}
