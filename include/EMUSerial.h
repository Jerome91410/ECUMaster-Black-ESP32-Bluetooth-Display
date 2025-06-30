// inspired by https://github.com/designer2k2/EMUcan

#ifndef _EMUSerial_h
#define _EMUSerial_h

#include <stdexcept>
#include <string>
using namespace std;

#include "Arduino.h"

struct emu_data_t {
  uint16_t RPM;                  // RPM
  uint16_t MAP;                  // kPa
  uint8_t TPS;                   // %
  int8_t IAT;                    // C
  float Batt;                    // V
  int8_t IgnAngle;               // deg
  float pulseWidth;              // ms
  uint16_t EGT1;                 // C
  uint16_t EGT2;                 // C
  float knockLevel;              // V
  float dwellTime;               // ms
  float wboAFR;                  // AFR
  int8_t gear;                   //
  uint8_t Baro;                  // kPa
  float analogIn1;               // V
  float analogIn2;               // V
  float analogIn3;               // V
  float analogIn4;               // V
  float analogIn5;               // V
  float analogIn6;               // V
  float injDC;                   // %
  int8_t emuTemp;                // C
  float oilPressure;             // Bar
  uint8_t oilTemperature;        // C
  float fuelPressure;            // Bar
  int16_t CLT;                   // C
  float flexFuelEthanolContent;  // %
  int8_t ffTemp;                 // C
  float wboLambda;               // λ
  uint16_t vssSpeed;             // km/h
  uint16_t deltaFPR;             // KPa
  int8_t fuelLevel;              // %
  uint8_t tablesSet;             // tabel
  float lambdaTarget;            // λ
  float secondaryPulseWidth;     // ms
  uint16_t cel;
  uint16_t knockCount;
};

class EMUSerial {
 public:
  EMUSerial();
  struct emu_data_t emu_data;

  void init();
  void onReceive(const uint8_t *buffer, size_t size);
  string decodeCel();
  bool isCelOn();

  enum ERRORFLAG : uint16_t {
    ERR_CLT = (1 << 0),         // Coolant temperature sensor failed
    ERR_IAT = (1 << 1),         // IAT sensor failed
    ERR_MAP = (1 << 2),         // MAP sensor failed
    ERR_WBO = (1 << 3),         // Wide band oxygen sensor failed
    ERR_EGT1 = (1 << 4),        // EGT sensor #1 failed
    ERR_EGT2 = (1 << 5),        // EGT sensor #2 failed
    EGT_ALARM = (1 << 6),       // EGT too high
    KNOCKING = (1 << 7),        // Knock detected
    FFSENSOR = (1 << 8),        // Flex Fuel sensor failed
    ERR_DBW = (1 << 9),         // Drive by wire failure
    ERR_FPR = (1 << 10),        // Fuel pressure relative error
    ERR_DIFF_CTRL = (1 << 11),  //
    ERR_DSG = (1 << 12),        // DSG
    ERR_EWG = (1 << 13),        // EWG
    ERR_OILP = (1 << 14)        // Oil pressure
  };

  static const uint16_t EFLG_ERRORMASK = ERR_CLT | ERR_IAT | ERR_MAP | ERR_WBO | ERR_EGT1 | ERR_EGT2 | EGT_ALARM | KNOCKING;
};

#endif
