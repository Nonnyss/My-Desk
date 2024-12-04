#ifndef RELAY_H
#define RELAY_H

void initRelay(int pin);
void controlRelay(bool state);

void initRelay2(int pin);
void controlRelay2(bool state);
bool isLightOn();

#endif
