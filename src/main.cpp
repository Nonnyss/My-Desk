#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "backlight.h"
#include "relay.h"
#include "distance.h"
#include "temp_hum.h"
#include "Arduino.h"
#include "motion.h"
#include <DHT.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "vector"
#define SDA_PIN 26
#define SCL_PIN 25
#define RELAY_PIN 18
#define RELAY_PIN_2 14
#define MOTION_SENSOR_PIN 35
#define TRIGGER_PIN 33
#define ECHO_PIN 32
#define DHT_PIN 27

const unsigned long lightCheckInterval = 10000;
const unsigned long logInterval = 2000;
const unsigned long displayUpdateInterval = 1000;

unsigned long lastLightCheck = 0;
unsigned long lastLogTime = 0;
unsigned long lastDisplayUpdate = 0;

void logSensorData(float temperature, float humidity, long distance, bool motion)
{
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    Serial.print("Motion: ");
    Serial.println(motion);
}

void controlLightTask(void *pvParameters)
{
    std::vector<long> duplicate;
    const double stableCount = 20;
    const double maxSize = 30;
    const double errorRate = 0;
    const double changeThreshold = 30;
    int previousState = 0;

    while (1)
    {
        long distance = readDistance();

        if (distance >= 2000)
        {
            duplicate.clear();
            duplicate.push_back(distance);
            controlRelay(true);
            controlRelay2(true);
            Serial.println("Relay ON (Distance >= 2000)");
            vTaskDelay(10000 / portTICK_PERIOD_MS);
            continue;
        }

        if (!isLightOn() && abs(previousState - distance) > changeThreshold)
        {
            if (duplicate.empty() || abs(duplicate.back() - distance) > errorRate)
            {
                duplicate.clear();
                duplicate.push_back(distance);
                controlRelay(true);
                controlRelay2(true);
                Serial.println("Relay ON (Significant Change)");
            }
            else
            {
                duplicate.push_back(distance);
                if (duplicate.size() > maxSize)
                {
                    duplicate.erase(duplicate.begin());
                }

                if (duplicate.size() >= stableCount)
                {
                    controlRelay(false);
                    controlRelay2(false);
                    Serial.println("Relay OFF (Stability Reached)");
                }
            }
        }
        else
        {
            duplicate.push_back(distance);

            if (duplicate.size() > maxSize)
            {
                duplicate.erase(duplicate.begin());
            }

            if (duplicate.size() >= stableCount)
            {
                controlRelay(false);
                controlRelay2(false);
                Serial.println("Relay OFF (Stable Readings)");
            }
        }

        previousState = distance;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void logDataTask(void *pvParameters)
{
    while (1)
    {
        unsigned long currentMillis = millis();

        if (currentMillis - lastLogTime >= logInterval)
        {
            float temperature = readTemperature();
            float humidity = readHumidity();
            long distance = readDistance();
            bool motion = readMotion();
            logSensorData(temperature, humidity, distance, motion);
            lastLogTime = currentMillis;
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void updateDisplayTask(void *pvParameters)
{
    while (1)
    {
        unsigned long currentMillis = millis();

        if (currentMillis - lastDisplayUpdate >= displayUpdateInterval)
        {
            float temperature = readTemperature();
            float humidity = readHumidity();

            if (!isnan(temperature) && !isnan(humidity))
            {
                clearDisplay();
                displayMessage("Temp: " + String(temperature - 2) + " C", 0, 0);
                displayMessage("Humi: " + String(humidity) + " %", 0, 1);
            }
            else
            {
                clearDisplay();
                displayMessage("Failed to", 0, 0);
                displayMessage("read sensor", 0, 1);
            }

            lastDisplayUpdate = currentMillis;
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void setup()
{
    Serial.begin(115200);

    initBacklight(SDA_PIN, SCL_PIN);
    initRelay(RELAY_PIN);
    initRelay2(RELAY_PIN_2);
    initDistanceSensor(TRIGGER_PIN, ECHO_PIN);
    initTempHumSensor(DHT_PIN);
    initMotionSensor(MOTION_SENSOR_PIN);

    displayMessage("System Initializing...");

    controlRelay(true);
    controlRelay2(true);
    clearDisplay();
    xTaskCreate(controlLightTask, "Control Light Task", 2048, NULL, 1, NULL);
    xTaskCreate(logDataTask, "Log Data Task", 2048, NULL, 1, NULL);
    xTaskCreate(updateDisplayTask, "Update Display Task", 2048, NULL, 1, NULL);
}

void loop()
{
}