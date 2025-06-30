

#include <EMUSerial.h>

EMUSerial::EMUSerial() {
  init();
}

void EMUSerial::init() {
  emu_data.analogIn1 = 0;
  emu_data.RPM = 0;
  emu_data.knockCount = 0;
}

void EMUSerial::onReceive(const uint8_t *buffer, size_t size) {
  // This decodes the frames and fills them into the data:

  uint8_t channel;
  uint8_t idChar;
  uint8_t valueH;
  uint8_t valueL;
  uint8_t checksum;
  uint16_t value;

  if (size % 5 > 0) {
    // discard buffers as the data might be corrupted due to data drop/rx busy
    return;
  }
  for (size_t i = 0; i < size;) {
    channel = buffer[i];
    valueH = buffer[i + 2];
    valueL = buffer[i + 3];
    value = (valueH << 8) | valueL;
    checksum = buffer[i + 4];  // TODO: should be checked
    // Serial.printf("channel %d - value %d - checksum %d\n", channel, value, checksum);
    // Serial.printf("msg: %x %x %x %x %x\n", channel, idChar, valueH, valueL, checksum);
    if (channel == 1) {
      emu_data.RPM = value;
    } else if (channel == 2) {
      emu_data.MAP = value;
    } else if (channel == 3) {
      emu_data.TPS = static_cast<uint8_t>(value);
    } else if (channel == 4) {
      emu_data.IAT = static_cast<int8_t>(value);
    } else if (channel == 5) {
      emu_data.Batt = static_cast<float>(value) / 37;
    } else if (channel == 6) {
      emu_data.IgnAngle = static_cast<int8_t>(value / 2);
    } else if (channel == 7) {
      emu_data.pulseWidth = static_cast<float>(value) / 62;
    } else if (channel == 8) {
      emu_data.EGT1 = value;
    } else if (channel == 9) {
      emu_data.EGT2 = value;
    } else if (channel == 10) {
      emu_data.knockLevel = static_cast<float>(value) / 51;
    } else if (channel == 11) {
      emu_data.dwellTime = static_cast<float>(value) / 20;
    } else if (channel == 12) {
      emu_data.wboAFR = static_cast<float>(value) / 10;
    } else if (channel == 13) {
      emu_data.gear = static_cast<int8_t>(value);
    } else if (channel == 14) {
      emu_data.Baro = static_cast<int8_t>(value);
    } else if (channel == 15) {
      emu_data.analogIn1 = static_cast<float>(value) / 51;
    } else if (channel == 16) {
      emu_data.analogIn2 = static_cast<float>(value) / 51;
    } else if (channel == 17) {
      emu_data.analogIn3 = static_cast<float>(value) / 51;
    } else if (channel == 18) {
      emu_data.analogIn4 = static_cast<float>(value) / 51;
    } else if (channel == 19) {
      emu_data.injDC = static_cast<float>(value) / 2;
    } else if (channel == 20) {
      emu_data.emuTemp = static_cast<int8_t>(value);
    } else if (channel == 21) {
      emu_data.oilPressure = static_cast<float>(value) / 16;
    } else if (channel == 22) {
      emu_data.oilTemperature = static_cast<int8_t>(value);
    } else if (channel == 23) {
      emu_data.fuelPressure = static_cast<float>(value) / 16;
    } else if (channel == 24) {
      emu_data.CLT = value;
    } else if (channel == 25) {
      emu_data.flexFuelEthanolContent = static_cast<float>(value) / 2;
    } else if (channel == 26) {
      emu_data.ffTemp = static_cast<int8_t>(value);
    } else if (channel == 27) {
      emu_data.wboLambda = static_cast<float>(value) / 128;
    } else if (channel == 28) {
      emu_data.vssSpeed = value / 4;
    } else if (channel == 29) {
      emu_data.deltaFPR = value;
    } else if (channel == 30) {
      emu_data.fuelLevel = static_cast<int8_t>(value);
    } else if (channel == 31) {
      emu_data.tablesSet = static_cast<int>(value);
    } else if (channel == 32) {
      emu_data.lambdaTarget = static_cast<float>(value) / 100;
    } else if (channel == 33) {
      emu_data.secondaryPulseWidth = static_cast<float>(value) / 62;
    } else if (channel == 34) {
      emu_data.analogIn5 = static_cast<float>(value) / 51;
    } else if (channel == 35) {
      emu_data.analogIn6 = static_cast<float>(value) / 51;
    } else if (channel == 255) {
      emu_data.cel = value;
    }
    i += 5;
  }
}

bool EMUSerial::isCelOn() {
  return emu_data.cel & EFLG_ERRORMASK;
}

string EMUSerial::decodeCel() {
  // TODO: move it in a dedicated file like emuBluetooth.cpp
  string cel_errors = "";
  if (isCelOn()) {
    if (emu_data.cel & ERR_OILP) {
      cel_errors += "OIL ";
    }
    if (emu_data.cel & ERR_CLT) {
      cel_errors = "CLT ";
    }
    if (emu_data.cel & ERR_IAT) {
      cel_errors += "IAT ";
    }
    if (emu_data.cel & ERR_MAP) {
      cel_errors += "MAP ";
    }
    if (emu_data.cel & ERR_WBO) {
      cel_errors += "WBO ";
    }
    if (emu_data.cel & ERR_EGT1) {
      cel_errors += "EGT1 ";
    }
    if (emu_data.cel & ERR_EGT2) {
      cel_errors += "EGT2 ";
    }
    if (emu_data.cel & EGT_ALARM) {
      cel_errors += "EGT ";
    }
    if (emu_data.cel & KNOCKING) {
      cel_errors += "KNOCK ";
      emu_data.knockCount++;
    }
    if (emu_data.cel & ERR_WBO) {
      cel_errors += "WBO ";
    }
    if (emu_data.cel & FFSENSOR) {
      cel_errors += "FF ";
    }
    if (emu_data.cel & ERR_DBW) {
      cel_errors += "DBW ";
    }
    if (emu_data.cel & ERR_FPR) {
      cel_errors += "FPR ";
    }
    if (emu_data.cel & ERR_DIFF_CTRL) {
      cel_errors += "DIFF ";
    }
    if (emu_data.cel & ERR_DSG) {
      cel_errors += "DSG ";
    }
    if (emu_data.cel & ERR_EWG) {
      cel_errors += "EWG ";
    }
    if (emu_data.cel & ERR_FPR) {
      cel_errors += "FPR ";
    }
  }
  return cel_errors;
}