# ESP32 Robot Dog

<p align="center">
  <img src="assets/img/small.jpg" alt="ESP32 Robot Dog" width="600"/>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32-blue?style=flat-square"/>
  <img src="https://img.shields.io/badge/IDE-Arduino-teal?style=flat-square"/>
  <img src="https://img.shields.io/badge/DOF-12-orange?style=flat-square"/>
  <img src="https://img.shields.io/badge/Control-WiFi%20Web%20UI-purple?style=flat-square"/>
  <img src="https://img.shields.io/badge/License-MIT-green?style=flat-square"/>
</p>

A 12-DOF quadruped robot dog powered by a single ESP32. Controlled via a browser-based dual-joystick interface over WiFi — no additional PWM boards, Bluetooth modules, or microcontrollers required.

---

## Features

- Single ESP32 — no extra hardware needed
- Browser-based dual-joystick control (phone / tablet / PC)
- True inverse kinematics per leg
- Configurable gait sequences (trot, creep)
- Serial CLI for calibration and debug
- Optional INA219 power monitoring
- Lightweight 3D-printed PLA body

---

## Repository Structure

```
.
├── LICENSE
├── README.md
├── assets
│   └── img
│       └── small.jpg
├── software
│   ├── robot_dog_esp32
│   │   ├── robot_dog_esp32.ino     # Main sketch
│   │   ├── def.h                   # Core definitions
│   │   ├── config.h                # Global config
│   │   ├── config_small.h          # Small robot config
│   │   ├── config_wifi.example.h   # WiFi credentials template
│   │   ├── subscription.h          # Subscription types
│   │   ├── cli.h                   # CLI declarations
│   │   ├── HAL.ino                 # Hardware abstraction layer
│   │   ├── HAL_ESP32PWM.ino        # ESP32 native PWM driver
│   │   ├── HAL_PCA9685.ino         # PCA9685 I2C PWM driver
│   │   ├── WiFi.ino                # WiFi AP setup
│   │   ├── webServer.ino           # Async web server
│   │   ├── packagesProcess.ino     # Control packet processing
│   │   ├── cli.ino                 # CLI main handler
│   │   ├── cliGet.ino              # CLI get commands
│   │   ├── cliSet.ino              # CLI set / trim commands
│   │   ├── gait.ino                # Gait loop
│   │   ├── servo.ino               # Servo write helpers
│   │   ├── settings.ino            # Runtime settings
│   │   ├── subscription.ino        # Pub/sub event system
│   │   ├── helpers.ino             # Utility functions
│   │   ├── failsafe.ino            # Watchdog / failsafe
│   │   ├── powerSensor.ino         # INA219 power monitoring
│   │   ├── imu.ino                 # IMU read (WIP)
│   │   ├── i2cscan.ino             # I2C bus scanner
│   │   ├── testHAL.ino             # HAL self-test
│   │   ├── libs
│   │   │   ├── HAL_body            # Body pose HAL
│   │   │   ├── IK                  # Inverse kinematics engine
│   │   │   ├── balance             # Balance controller
│   │   │   ├── gait                # Gait library
│   │   │   ├── planner             # Motion planner
│   │   │   └── transition          # State transition manager
│   │   └── web
│   │       └── index.html.gz.h     # Compressed web UI (PROGMEM)
│   └── web
│       ├── README.md               # Web UI build instructions
│       ├── gulpfile.js             # Build pipeline
│       ├── package.json
│       └── src
│           ├── index.html          # Web UI source
│           └── s.js                # Joystick + telemetry JS
└── tools
    └── servoCalib
        └── servoCalib.ino          # Standalone servo calibration sketch
```

---

## Hardware

### Bill of Materials

| Component | Qty | Notes |
|-----------|-----|-------|
| ESP32 (38-pin) | 1 | Main controller |
| TowerPro MG90D / MG90S Servo | 12 | Cable exit at bottom |
| Mini360 DC-DC Buck Converter | 3 | 2× servos (5V) · 1× ESP32 (3.3V) |
| 18650 Battery Holder (2S SMT) | 1 | |
| 18650 Battery | 2 | 3.7V · 2000mAh+ |
| INA219 Current Sensor | 1 | Optional |
| Prototyping Board 50×70mm | 1 | |
| Bearing 8×12×2.5mm | 8 | Shoulder joints |
| Cyanoacrylate (super glue) | 1 | Body sub-assembly bonding |

### Servo GPIO Map

| Leg | Alpha (Hip Yaw) | Beta (Shoulder) | Gamma (Knee) |
|-----|-----------------|-----------------|--------------|
| Left Front  | 25 | 26 | 27 |
| Right Front | 16 | 18 | 17 |
| Left Hind   | 13 | 12 | 14 |
| Right Hind  |  4 |  2 | 15 |

I²C: **SDA → GPIO 21 · SCL → GPIO 22**

### Power

```
2× 18650 in series (~8.4V)
  ├── Mini360 #1 → 5.0V → Left servos  (×6)
  ├── Mini360 #2 → 5.0V → Right servos (×6)
  └── Mini360 #3 → 3.3V → ESP32
```

---

## 3D Printed Parts

Print files are not included in this repo. See the Thingiverse link in the project page.

| Part | Qty | Mirror | Material |
|------|-----|--------|----------|
| Body | 1 | — | PLA |
| Leg Top | 2 + 2 | ✅ | PLA |
| Leg Bottom | 2 + 2 | ✅ | PLA |
| Shoulder Part 1 | 4 | — | PLA |
| Shoulder Part 2 | 4 | — | PLA |
| Legs Holder Part 1 | 2 | — | PLA |
| Legs Holder Part 2 | 2 | — | PLA |
| Servo Gear | 4 | — | PLA |
| Cover | 1 | — | PLA |
| Cover Clamps | 2 | — | PLA |
| Leg Shoes | 4 | — | **TPU** |
| Calib Tool Beta/Gamma | 1 + 1 | ✅ | PLA |
| Calib Tool Alpha | 1 | — | PLA |
| Calib Servo Disc 10° | 1 | — | PLA |

**Recommended print settings:** 0.12mm layer height · 30% infill · Support on · Brim on · Ironing on

---

## Software Setup

### Dependencies

| Library | Version | How to install |
|---------|---------|---------------|
| ESP Async Web Server | latest | [GitHub](https://github.com/me-no-dev/ESPAsyncWebServer) — manual |
| AsyncTCP | latest | [GitHub](https://github.com/me-no-dev/AsyncTCP) — manual |
| ESP32 ISR Servo | **1.1.0** | Arduino Library Manager |
| MPU9250_WE | **1.1.3** | Arduino Library Manager |
| INA219_WE | latest | Arduino Library Manager |

> ⚠️ Use ESP32 Arduino core **v1.x only**. v2.x causes a WiFi-related core freeze.

### WiFi Configuration

```bash
cp software/robot_dog_esp32/config_wifi.example.h \
   software/robot_dog_esp32/config_wifi.h
```

Edit `config_wifi.h`:

```cpp
const char* APssid = "RobotDog";      // AP name
const char* APpass = "yourpassword";  // Min 8 chars
```

### Flash

1. Open `software/robot_dog_esp32/robot_dog_esp32.ino` in Arduino IDE
2. Board: **ESP32 Dev Module**
3. Upload speed: **921600**
4. Click **Upload**

### Web UI (optional rebuild)

```bash
cd software/web
npm install
npm run build        # outputs index.html.gz.h into robot_dog_esp32/web/
```

---

## Usage

1. Power on the robot
2. Connect to WiFi AP: **RobotDog**
3. Open `http://192.168.4.1` in any browser
4. Use the dual joystick to control movement
5. Telemetry (battery voltage / current) shown live if INA219 is fitted

---

## Serial CLI

Open Arduino Serial Monitor at **115200 baud**.

```bash
set help                     # list all commands
set servo_to_calib           # center all servos (calibration position)
set LF_HAL_trim_alpha -3     # trim left-front alpha joint by -3°
set RF_HAL_trim_beta 2
get telemetry                # print live telemetry
```

**Leg prefixes:** `LF` · `RF` · `LH` · `RH`  
**Joint names:** `alpha` · `beta` · `gamma`

---

## Calibration

### 1 — Servo Limits

Flash `tools/servoCalib/servoCalib.ino` to find `minUs` / `maxUs` for your servo batch.  
Connect one servo to GPIO 14. Send µs values over Serial.

```
1500  →  centre
800   →  decrease until stall, step back one value = minUs
2200  →  increase until stall, step back one value = maxUs
```

### 2 — Leg Assembly

Send `set servo_to_calib` via Serial CLI before assembling legs.  
Use the printed calibration tools to align each joint at 90° / 45° / 90°.

### 3 — Fine Trim

Use `set XX_HAL_trim_YYY <value>` to dial in small offsets.  
Persist final values by editing `config_small.h`.

---

## License

MIT — see [LICENSE](LICENSE).