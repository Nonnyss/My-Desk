#include <Arduino.h>
#include "distance.h"

int triggerPin;
int echoPin;

void initDistanceSensor(int trigPin, int echPin) {
    triggerPin = trigPin;
    echoPin = echPin;
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);

}

long readDistance() {
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    
    long duration = pulseIn(echoPin, HIGH);
    long distance = duration * 0.034 / 2;
    return distance;
}
