#include <Arduino.h>
#include <BluetoothSerial.h>
#include <TaskManagerIO.h>

#include <stdexcept>
#include <string>
using namespace std;

#include <EMUSerial.h>
#include <displayControl.h>

#include "emu_mac_address.h"
#include "ledControl.h"

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;
EMUSerial emuSerial;

// #define USE_NAME
const char *pin = "1234";
String myBtName = "ESP32-BT-Master";

#ifdef USE_NAME
String slaveName = "EMUCANBT_SPP";  // not recommended
#else
uint8_t address[6] = {MAC0, MAC1, MAC2, MAC3, MAC4, MAC5};  // setup the value in lib/emu_mac_address.h
#endif

// #define BUZZ_ALARM
const int buzzerPin = 22;
bool buzzerOn = false;

const unsigned long reconnectInterval = 5000;
// Display & LVGL setup
lv_obj_t *table;

void connectToBt() {
  bool connected;
  if (!SerialBT.hasClient()) {
    ledLightBlue();
#ifdef USE_NAME
    connected = SerialBT.connect(slaveName);
#else
    connected = SerialBT.connect(address);
#endif
    if (connected) {
      Serial.println("Connected Successfully!");
      ledBlue();
    } else {
      Serial.println("Initial connect failed. Will retry in loop...");
      ledRed();
      delay(1000);
    }
  }
}

const int maxRPM = 7200;
const int shiftlightRPM1 = maxRPM * 0.88;
const int shiftlightRPM2 = maxRPM * 0.95;
void refreshLed() {
  if (!SerialBT.hasClient()) {
    // we should reinit all values
    emuSerial.emu_data.cel = 0;
    ledBlinkBlue();
  } else {
    if (emuSerial.emu_data.cel > 0) {
      ledRed();
    } else {
      if (emuSerial.emu_data.RPM < shiftlightRPM1) {
        ledBlue();
      } else if (emuSerial.emu_data.RPM > shiftlightRPM2) {
        ledFastBlinkWhite();
      } else {
        ledWhite();
      }
    }
  }
}

void callbackReadBTData(const uint8_t *buffer, size_t size) {
  emuSerial.onReceive(buffer, size);
}

void refreshDisplay() {
  renderDisplay(table, emuSerial);
}

void buzzAlarms() {
  float boost = (static_cast<float>(emuSerial.emu_data.MAP) / 100) - 1.0132f;
  buzzerOn = (emuSerial.emu_data.cel > 0 || emuSerial.emu_data.CLT > 105 || emuSerial.emu_data.RPM > 7000 || boost > 1.10 || (emuSerial.emu_data.Batt < 12.00 && emuSerial.emu_data.Batt > 1.00));
  digitalWrite(buzzerPin, (millis() % 600 < 300) && buzzerOn);
}

void loop() {
  taskManager.runLoop();
}

void setup() {
  // init Serial
  Serial.begin(115200);
  Serial.println("Serial init");
  Serial.printf("maxRPM: %d\n", maxRPM);
  Serial.printf("shiftlightRPM1: %d\n", shiftlightRPM1);
  Serial.printf("shiftlightRPM2: %d\n", shiftlightRPM2);

  // Init led
  initLed();
  // set grid
  table = initDisplay();
  // init bluetooth
  SerialBT.begin(myBtName, true);
#ifndef USE_NAME
  SerialBT.setPin(pin);
#endif
  SerialBT.onData(callbackReadBTData);

  // init buzzer
  pinMode(buzzerPin, OUTPUT);

  // init tasks
  taskManager.yieldForMicros(300);
  taskManager.scheduleFixedRate(300, refreshLed);
  taskManager.scheduleFixedRate(300, refreshDisplay);
  taskManager.scheduleFixedRate(reconnectInterval, connectToBt);

#ifdef BUZZ_ALARM
  taskManager.scheduleFixedRate(300, buzzAlarms);
#endif
}
