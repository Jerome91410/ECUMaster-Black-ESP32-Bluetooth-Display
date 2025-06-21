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

// #define USE_NAME
const char *pin = "1234";
String myBtName = "ESP32-BT-Master";

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;
EMUSerial emuSerial;

#ifdef USE_NAME
String slaveName = "EMUCANBT_SPP";  // not recommended
#else
uint8_t address[6] = {MAC0, MAC1, MAC2, MAC3, MAC4, MAC5};  // setup the value in lib/emu_mac_address.h
#endif

const int buzzerPin = 22;
bool buzzerOn = false;
const unsigned long reconnectInterval = 5000;
// Display & LVGL setup
lv_obj_t *table;

void connectToBt() {
  bool connected;
  if (!SerialBT.hasClient()) {
    ledWhite();
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

void ledStatus() {
  if (!SerialBT.hasClient()) {
    // we should reinit all values
    emuSerial.emu_data.cel = 0;
    ledBlinkBlue();
  } else {
    if (emuSerial.emu_data.cel > 0) {
      ledRed();
    } else {
      ledBlue();
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
  float boost = (emuSerial.emu_data.MAP / 100) - 1.0132f;
  buzzerOn = (emuSerial.emu_data.cel > 0 || emuSerial.emu_data.CLT > 105 || emuSerial.emu_data.RPM > 7000 || boost > 1.10 || (emuSerial.emu_data.Batt < 12.00 && emuSerial.emu_data.Batt > 1.00));
  digitalWrite(buzzerPin, (millis() % 600 < 300) && buzzerOn);
}

void loop() {
  taskManager.runLoop();
}

void setup() {
  // init Serial
  Serial.begin(115200);
  Serial.println("setup!");

  // Init led
  initLed();
  ledOff();
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
  taskManager.yieldForMicros(250);
  taskManager.scheduleFixedRate(250, ledStatus);
  taskManager.scheduleFixedRate(300, refreshDisplay);
  taskManager.scheduleFixedRate(300, buzzAlarms);
  taskManager.scheduleFixedRate(reconnectInterval, connectToBt);
}
