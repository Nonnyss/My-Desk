#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "backlight.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void initBacklight(int sda, int scl)
{
    lcd.init(sda, scl);
    lcd.backlight();
}

void displayMessage(const String &message , int row , int col)
{
    lcd.setCursor(row, col);
    lcd.print(message);
}

void clearDisplay()
{
    lcd.clear();
}