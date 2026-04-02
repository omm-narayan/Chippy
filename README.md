# ESP32 Robot Dog

<p align="center">
  <img src="assets/renders/robot-dog-banner.png" alt="ESP32 Robot Dog" width="100%"/>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32-blue?style=flat-square"/>
  <img src="https://img.shields.io/badge/IDE-Arduino-teal?style=flat-square"/>
  <img src="https://img.shields.io/badge/DOF-12-orange?style=flat-square"/>
  <img src="https://img.shields.io/badge/Control-WiFi%20Web%20UI-purple?style=flat-square"/>
  <img src="https://img.shields.io/badge/License-MIT-green?style=flat-square"/>
</p>

A **12-DOF quadruped robot dog** powered entirely by a single ESP32. Controlled via a browser-based joystick interface over WiFi — no additional microcontrollers, Bluetooth modules, or PWM ICs required.

---

## Table of Contents

- [Features](#features)
- [Demo](#demo)
- [Hardware](#hardware)
  - [Electronics](#electronics)
  - [3D Printed Parts](#3d-printed-parts)
  - [Wiring](#wiring)
- [Software](#software)
  - [Dependencies](#dependencies)
  - [Configuration](#configuration)
  - [Upload](#upload)
- [Assembly](#assembly)
- [Calibration](#calibration)
- [Web Interface](#web-interface)
- [Folder Structure](#folder-structure)
- [License](#license)

---

## Features

- ✅ Single ESP32 — no extra PWM boards or Bluetooth modules
- ✅ Browser-based dual-joystick control (phone / tablet / PC)
- ✅ True Inverse Kinematics per leg
- ✅ Configurable gait sequences
- ✅ CLI interface via Serial for calibration and debug
- ✅ Optional INA219 power monitoring
- ✅ Lightweight 3D-printed PLA body

---

## Demo

| Render | Walking Gait |
|--------|-------------|
| ![render](assets/renders/side-view.png) | ![demo](assets/demo/walk-preview.gif) |

> 📽️ Full Fusion 360 animated assembly walkthrough: [`assets/renders/assembly-animation.mp4`](assets/renders/assembly-animation.mp4)

---

## Hardware

### Electronics

| Component | Qty | Notes |
|-----------|-----|-------|
| ESP32 (38-pin) | 1 | Main controller |
| TowerPro MG90D / MG90S Servo | 12 | Cable exit at bottom |
| Mini360 DC-DC Buck Converter | 3 | 2× for legs, 1× for ESP32 |
| 18650 Battery Holder (2S) | 1 | SMT type preferred |
| 18650 Battery | 2 | 3.7V, 2000mAh+ |
| INA219 Current Sensor | 1 | Optional — power telemetry |
| Prototyping Board (50×70mm) | 1 | For PCB assembly |
| Bearings 8×12×2.5mm | 8 | Shoulder joints |
| Electrolytic Capacitors | 4–6 | Noise decoupling |

> ⚠️ Servo sizing matters. MG90D/S have slightly different dimensions from generic clones. Verify before printing.

---

### 3D Printed Parts

All STL files are in [`hardware/3d-models/`](hardware/3d-models/).

| Part | Qty | Mirror? | Notes |
|------|-----|---------|-------|
| Body | 1 | — | |
| Leg Top | 2 | ✅ (×2 mirrored) | |
| Leg Bottom | 2 | ✅ (×2 mirrored) | |
| Shoulder Part 1 | 4 | — | |
| Shoulder Part 2 | 4 | — | |
| Legs Holder Part 1 | 2 | — | |
| Legs Holder Part 2 | 2 | — | |
| Servo Gear | 4 | — | Print at highest quality |
| Leg Cover | 1 | — | |
| Cover Clamps | 2 | — | |
| Leg Shoes | 4 | — | **Flexible filament (TPU)** |
| Calibration Tool (Beta/Gamma) | 1 | ✅ (×1 mirrored) | |
| Calibration Tool (Alpha) | 1 | — | |
| Servo Calibration Disc (10°) | 1 | — | |

**Print Settings (Ender 3 / Cura):**

```
Layer Height  : 0.12mm (Dynamic Quality)
Infill        : 30–40%
Support       : Yes (Touching Build Plate)
Brim          : Yes (5mm)
Ironing        : Yes (top surfaces)
Material      : PLA
```

> 🦶 Servo gears must be printed as precisely as possible. Enable Support for "floating" geometry.

---

### Wiring

Servo GPIO map (configurable in `firmware/src/config.h`):

| Leg | Alpha (Body) | Beta (Shoulder) | Gamma (Knee) |
|-----|-------------|-----------------|--------------|
| Left Front  | GPIO 25 | GPIO 26 | GPIO 27 |
| Right Front | GPIO 16 | GPIO 18 | GPIO 17 |
| Left Hind   | GPIO 13 | GPIO 12 | GPIO 14 |
| Right Hind  | GPIO 4  | GPIO 2  | GPIO 15  |

I²C (IMU + INA219): **SDA → GPIO 21**, **SCL → GPIO 22**

Full schematic: [`hardware/schematics/wiring.pdf`](hardware/schematics/wiring.pdf)

---

## Software

### Dependencies

Install via Arduino IDE → *Sketch → Include Library → Manage Libraries*:

| Library | Version | Install Method |
|---------|---------|----------------|
| ESP Async Web Server | latest | Manual / GitHub |
| AsyncTCP | latest | Manual / GitHub |
| ESP32 ISR Servo | **1.1.0** | Library Manager |
| MPU9250_WE | **1.1.3** | Library Manager |
| INA219_WE | latest | Library Manager |

> ⚠️ Use ESP32 board package **v1.x** only. v2.x introduced WiFi task changes that cause core freezes.

Add this to Arduino `boards.txt` URL:
```
https://dl.espressif.com/dl/package_esp32_index.json
```

---

### Configuration

1. Rename `firmware/src/config_wifi.example.h` → `config_wifi.h`
2. Set your credentials:

```cpp
const char* APssid = "RobotDog";       // WiFi AP name
const char* APpass = "yourpassword";   // Min 8 characters
```

3. Tune servo limits in `firmware/src/config.h` if needed.

---

### Upload

1. Open `firmware/src/main.ino` in Arduino IDE
2. Select Board: **ESP32 Dev Module**
3. Select Port: your ESP32's COM port
4. Click **Upload**

---

## Assembly

See the step-by-step guide: [`docs/assembly.md`](docs/assembly.md)

High-level order:

```
1. Print all parts → clean elephant foot from gears
2. Glue body sub-assemblies (flat file surfaces before gluing)
3. Cut servo horns → glue into servo gears
4. Thread servo cables through leg and body channels
5. Set all servos to center (1500µs) before gearing
6. Install shoulder bearings and squeeze-fit assemblies
7. Solder PCB → flash firmware → calibrate
```

---

## Calibration

Full guide: [`docs/calibration.md`](docs/calibration.md)

**Quick steps via Serial CLI:**

```bash
# Set all servos to calibration position
set servo_to_calib

# List all available trim commands
set help

# Trim individual joint (example: left-front alpha)
set LF_HAL_trim_alpha -3

# Trim left-front beta
set LF_HAL_trim_beta 2
```

Leg prefixes: `LF` `RF` `LH` `RH` (Left/Right Front/Hind)  
Angles: `alpha` (body rotate) · `beta` (shoulder) · `gamma` (knee)

---

## Web Interface

After flashing:

1. Connect to WiFi AP: **RobotDog** (or your configured name)
2. Open browser → `http://192.168.4.1`
3. Use dual joystick to control movement
4. View live telemetry (battery voltage via INA219)

Works on any modern browser — mobile optimized.

---

## Folder Structure

```
esp32-robot-dog/
├── firmware/
│   ├── src/
│   │   ├── main.ino               # Entry point
│   │   ├── config.h               # GPIO map, servo limits, tuning
│   │   ├── config_wifi.example.h  # WiFi credentials template
│   │   ├── kinematics.h           # Inverse kinematics engine
│   │   ├── gait.h                 # Gait sequencer
│   │   ├── servo_hal.h            # Servo hardware abstraction
│   │   └── webserver.h            # Async web server + UI
│   ├── lib/
│   │   └── dependencies.md        # Library version pinning
│   └── tools/
│       └── servo_calibration/
│           └── servo_calib.ino    # Standalone servo calibration sketch
│
├── hardware/
│   ├── 3d-models/
│   │   ├── body/
│   │   ├── legs/
│   │   ├── shoulder/
│   │   ├── assembly/
│   │   └── calibration-tools/
│   ├── schematics/
│   │   ├── wiring.pdf
│   │   └── wiring.png
│   └── pcb/
│       └── gerbers/               # (optional PCB export)
│
├── docs/
│   ├── assembly.md
│   ├── calibration.md
│   ├── electronics.md
│   └── images/
│
├── assets/
│   ├── renders/                   # Fusion 360 renders + animation
│   ├── demo/                      # GIFs, videos
│   └── screenshots/               # Web UI screenshots
│
├── .gitignore
├── LICENSE
└── README.md
```

---

## License

MIT License — see [`LICENSE`](LICENSE) for details.
