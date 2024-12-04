#include "DHT.h"
#include "temp_hum.h"

#define DHTTYPE DHT11

DHT dhtSensor(0, DHTTYPE); 

void initTempHumSensor(uint8_t pin) {
    dhtSensor = DHT(pin, DHTTYPE);
    dhtSensor.begin();
}

float readTemperature() {
    float temp = dhtSensor.readTemperature();
    if (isnan(temp)) {
        Serial.println("Failed to read temperature!");
        return NAN;
    }
    return temp;
}

float readHumidity() {
    float humidity = dhtSensor.readHumidity();
    if (isnan(humidity)) {
        Serial.println("Failed to read humidity!");
        return NAN;
    }
    return humidity;
}
