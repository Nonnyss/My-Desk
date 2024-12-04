#ifndef BACKLIGHT_H
#define BACKLIGHT_H

void initBacklight(int sda , int scl);
void displayMessage(const String& message , int row = 0 , int col = 0);
void clearDisplay();
#endif
