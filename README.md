# 🚗 ECUMaster Black + ESP32 Bluetooth Display

[![ESP32](https://img.shields.io/badge/board-ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![ECUMaster](https://img.shields.io/badge/device-ECUMaster-black.svg)](https://www.ecumaster.com)
[![Bluetooth](https://img.shields.io/badge/communication-Bluetooth-brightgreen.svg)](https://en.wikipedia.org/wiki/Bluetooth)
[![LVGL](https://img.shields.io/badge/UI-LVGL-purple.svg)](https://lvgl.io/)
[![Arduino](https://img.shields.io/badge/framework-Arduino-blue.svg)](https://www.arduino.cc)
[![License: GPL v3](https://img.shields.io/badge/license-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.en.html)
[![Language: C++](https://img.shields.io/badge/language-C++-orange.svg)](https://isocpp.org)
[![Vehicle Telemetry](https://img.shields.io/badge/type-Telemetry-lightgrey.svg)]()

---

### 📦 Description

This project interfaces the **ECUMaster Black ECU** with an **ESP32** to create a Bluetooth-enabled dashboard display, featuring:

- Automatic Bluetooth reconnection
- Real-time monitoring of engine vitals
- Custom alerts (CEL, coolant temp, RPM, AFR, voltage, boost, etc.) via display & buzzer
- TFT screen GUI with **LVGL**

> ⚠️ **Note:** If `SerialBT.setPin(pin)` doesn't work, downgrade to **ESP32 Arduino Core 2.0.17** in the board manager.

---

### ✅ Tested On

- ESP32 JC2432W328
- Active 3.3V buzzer
- Arduino Core v2.0.17
- ECUMaster Black with Bluetooth Adapter and EDL-1
- LVGL v8.3 or v8.4 (not compatible v9.X)

---

### 📥 Installation

1. Clone this repo
2. Open the project in VS-Code
3. Install recommended extensions like platform.io
4. Now you can build/upload and monitor via the platform.io extions. All dependencies are installed automatically
5. Upload to ESP32
6. Optional - Connect an active 3.0-3.3V buzzer to GPIO Pin 22 (for warnings/CEL)
7. Pair with ECUMaster Black BT adapter
8. Enjoy real-time data on-screen!

---

### LED Behavior

- FLashing blue: Looking for connection
- Light blue: trying to connect to your emu
- Blue: connected
- Red:  error occured
  - if not connected, failed to connect to your emu
  - if connected, check engine light ON
- White: 88% max rpm
- Flashing White: 95% max rpm

---

### 📦 Dependencies

- [BluetoothSerial](https://github.com/espressif/arduino-esp32/tree/master/libraries/BluetoothSerial)
- [LVGL](https://lvgl.io)
- [eSPI](https://github.com/Bodmer/TFT_eSPI)

--- 

### ❓ FAQ

**Q:** SerialBT.setPin() doesn't work?  
**A:** Use ESP32 Arduino Core v2.0.17. Later versions removed this feature.

**Q:** Display not initializing?  
**A:** Check eSPI & LVGL configuration or contact the manufacturer of your display for configuration guidelines.

**Q:** Blurry text?  
**A:** Check lv_conf.h - LV_COLOR_16_SWAP

**Q:** Font size not working?  
**A:** Check lv_conf.h - Enable the required font sizes. 

---

### 🧰 Hardware Used

- **ESP32 JC2432W328**  
  [🛒 AliExpress link](https://www.aliexpress.com/item/1005006729707613.html)
  
  Documentation: https://drive.google.com/drive/folders/1Sk4sIXgLAqPZ03BzYb0IwUftMXJ9QMLN?usp=sharing

- **3D Printed Case**  
  [📦 Thingiverse STL](https://www.thingiverse.com/thing:6705691)
- **ECU**  
ECUMaster Black - https://www.ecumaster.com/products/emu-black/

  EMU CAN Bluetooth Adapter - https://ecumasterusa.com/products/bluetooth-adapter-for-ecumaster-emu-can-bus

- **Active 3.3V buzzer**    
 [🛒 AliExpress link](https://www.aliexpress.com/item/1005008682347898.html)
  
---

## 📺 Demo Videos

[![Watch on YouTube](https://img.youtube.com/vi/b16--jYl7CY/0.jpg)](https://youtu.be/b16--jYl7CY)

[![Watch on YouTube](https://img.youtube.com/vi/1yv-feC6I2c/0.jpg)](https://youtu.be/1yv-feC6I2c)

---

### 📜 License

This project is licensed under **GPL v3**.  
See the [LICENSE](https://www.gnu.org/licenses/gpl-3.0.en.html) file for more info.

---

### ❤️ Credits & Contributions

Made with ❤️ for petrolheads.  
Contributions, forks, and feature requests are welcome!
