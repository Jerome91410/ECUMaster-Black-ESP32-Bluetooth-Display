#include <Arduino.h>
#include <ledControl.h>

LED_STATUS ledStatus = OFF;

void initLed() {
  pinMode(4, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(16, OUTPUT);
  ledOff();
}

void ledOff() {
  if (ledStatus != OFF) {
    digitalWrite(4, 1);
    digitalWrite(16, 1);
    digitalWrite(17, 1);
    ledStatus = OFF;
  }
}

void ledRed() {
  if (ledStatus != RED) {
    digitalWrite(4, 0);
    digitalWrite(16, 1);
    digitalWrite(17, 1);
    ledStatus = RED;
  }
}
void ledGreen() {
  if (ledStatus != GREEN) {
    digitalWrite(4, 1);
    digitalWrite(16, 0);
    digitalWrite(17, 1);
    ledStatus = GREEN;
  }
}

void ledLightBlue() {
  if (ledStatus != LIGHT_BLUE) {
    digitalWrite(4, 1);
    digitalWrite(16, 0);
    digitalWrite(17, 0);
    ledStatus = LIGHT_BLUE;
  }
}

void ledBlue() {
  if (ledStatus != BLUE) {
    digitalWrite(4, 1);
    digitalWrite(16, 1);
    digitalWrite(17, 0);
    ledStatus = BLUE;
  }
}

void ledWhite() {
  if (ledStatus != WHITE) {
    digitalWrite(4, 0);
    digitalWrite(16, 0);
    digitalWrite(17, 0);
    ledStatus = WHITE;
  }
}

void ledBlinkBlue() {
  if (ledStatus == OFF) {
    ledBlue();
  } else {
    ledOff();
  }
}

void ledFastBlinkWhite() {
  for (int i = 0; i < 3; i++) {
    ledWhite();
    delay(50);
    ledOff();
    delay(50);
  }
}
