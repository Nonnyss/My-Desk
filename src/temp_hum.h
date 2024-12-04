#ifndef TEMP_HUM_H
#define TEMP_HUM_H

void initTempHumSensor(uint8_t pin);
float readTemperature();
float readHumidity();

#endif
