#ifndef _LED_CONTROL_h
#define _LED_CONTROL_h

void initLed();
void ledOff();
void ledRed();
void ledGreen();
void ledBlue();
void ledLightBlue();
void ledBlinkBlue();
void ledWhite();
void ledFastBlinkWhite();

enum LED_STATUS {
  OFF = 0,
  RED,
  GREEN,
  BLUE,
  LIGHT_BLUE,
  WHITE,
};

#endif