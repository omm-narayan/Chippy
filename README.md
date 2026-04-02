# Chippy a ESP32 Robot Dog


A 12-DOF quadruped robot dog powered by a single ESP32. Controlled via a browser-based dual-joystick interface over WiFi — no additional PWM boards, Bluetooth modules, or microcontrollers required.

---

## Table of Contents

1. [What You Will Need](#1-what-you-will-need)
2. [Repository Structure](#2-repository-structure)
3. [3D Printing](#3-3d-printing)
4. [Hardware and Electronics](#4-hardware-and-electronics)
5. [Wiring](#5-wiring)
6. [Installing Software Tools](#6-installing-software-tools)
7. [Installing Arduino Libraries](#7-installing-arduino-libraries)
8. [Configuring the Firmware](#8-configuring-the-firmware)
9. [Flashing the ESP32](#9-flashing-the-esp32)
10. [Servo Calibration](#10-servo-calibration)
11. [Mechanical Assembly](#11-mechanical-assembly)
12. [Leg Calibration](#12-leg-calibration)
13. [First Boot and Web Control](#13-first-boot-and-web-control)
14. [Serial CLI Reference](#14-serial-cli-reference)
15. [Rebuilding the Web UI](#15-rebuilding-the-web-ui)
16. [Troubleshooting](#16-troubleshooting)

---

## 1. What You Will Need

### Tools

| Tool | Purpose |
|------|---------|
| FDM 3D printer (Ender 3 or similar) | Print all structural parts |
| Soldering iron + solder | PCB assembly |
| Multimeter | Verify voltages before powering servos |
| Flat needle file | Clean print seams on gluing surfaces |
| Small Phillips screwdriver (M2) | Servo mounting |
| Wire cutters and strippers | Servo cable prep |
| Tweezers | Insert bearings, route cables |
| Calipers (optional) | Verify bearing fits |
| USB-A to Micro-USB cable | Flash ESP32 |
| PC / Mac / Linux machine | Arduino IDE and web build |

### Electronics

| Component | Qty | Notes |
|-----------|-----|-------|
| ESP32 (38-pin dev board) | 1 | Any 38-pin variant |
| TowerPro MG90D or MG90S Servo | 12 | Cable exit must be at the **bottom** of the servo body |
| Mini360 DC-DC Buck Converter | 3 | 2x for servos at 5V · 1x for ESP32 at 3.3V |
| 18650 Battery Holder 2S SMT type | 1 | Search "18650 battery holder smt" |
| 18650 Battery 3.7V 2000mAh+ | 2 | Protected cells recommended |
| INA219 Current/Voltage Sensor | 1 | Optional — power telemetry |
| MPU9250 IMU | 1 | Optional — not yet implemented |
| Green prototyping board 50x70mm | 1 | Manual PCB base |
| Electrolytic capacitor 100µF | 4–6 | Power rail noise suppression |
| Ceramic capacitor 100nF | 4 | ESP32 power pin decoupling |
| Male and female pin headers | 1 set | ESP32 socket on proto board |
| Hookup wire 22–26 AWG | ~2m | Signal and power runs |
| Small toggle switch | 1 | Main power switch |

### Mechanical

| Item | Qty |
|------|-----|
| Bearing 8x12x2.5mm | 8 |
| M2x5mm self-tapping screw | ~20 |
| Small cable ties | 4 |
| Cyanoacrylate super glue | 1 tube |
| PLA filament | ~250g |
| TPU flexible filament | ~20g |

### Estimated Cost

| Category | USD (approx) |
|----------|-------------|
| Electronics | $25–35 |
| 12x Servos | $25–40 |
| Filament | $5–10 |
| Bearings and hardware | $5–10 |
| Batteries | $8–15 |
| **Total** | **~$68–110** |

---

## 2. Repository Structure

```
.
├── LICENSE
├── README.md
├── assets
│   └── img
│       └── small.jpg
├── software
│   ├── robot_dog_esp32
│   │   ├── robot_dog_esp32.ino     # Main sketch entry point
│   │   ├── def.h                   # Core constants and enums
│   │   ├── config.h                # Global robot configuration
│   │   ├── config_small.h          # Small robot geometry and servo profiles
│   │   ├── config_wifi.example.h   # WiFi credentials template
│   │   ├── subscription.h          # Event subscription types
│   │   ├── cli.h                   # CLI function declarations
│   │   ├── HAL.ino                 # Hardware abstraction layer
│   │   ├── HAL_ESP32PWM.ino        # ESP32 native PWM servo driver
│   │   ├── HAL_PCA9685.ino         # PCA9685 I2C PWM driver (optional)
│   │   ├── WiFi.ino                # WiFi access point setup
│   │   ├── webServer.ino           # Async web server and API routes
│   │   ├── packagesProcess.ino     # Control packet parser
│   │   ├── cli.ino                 # CLI dispatcher
│   │   ├── cliGet.ino              # CLI get commands
│   │   ├── cliSet.ino              # CLI set and trim commands
│   │   ├── gait.ino                # Gait update loop
│   │   ├── servo.ino               # Servo write and angle mapping
│   │   ├── settings.ino            # Runtime settings read and write
│   │   ├── subscription.ino        # Pub/sub event system
│   │   ├── helpers.ino             # Math and utility functions
│   │   ├── failsafe.ino            # Watchdog and failsafe stand
│   │   ├── powerSensor.ino         # INA219 voltage and current read
│   │   ├── imu.ino                 # IMU read (WIP)
│   │   ├── i2cscan.ino             # I2C bus scanner utility
│   │   ├── testHAL.ino             # HAL self-test routine
│   │   ├── libs
│   │   │   ├── HAL_body            # Body pose hardware abstraction
│   │   │   │   ├── HAL_body.cpp
│   │   │   │   └── HAL_body.h
│   │   │   ├── IK                  # Inverse kinematics engine
│   │   │   │   ├── IK.cpp
│   │   │   │   ├── IK.h
│   │   │   │   ├── IK_simple.cpp
│   │   │   │   ├── IK_simple.h
│   │   │   │   ├── geometry.h
│   │   │   │   └── leg.h
│   │   │   ├── balance             # Balance controller
│   │   │   │   ├── balance.cpp
│   │   │   │   └── balance.h
│   │   │   ├── gait                # Gait sequence library
│   │   │   │   ├── gait.cpp
│   │   │   │   └── gait.h
│   │   │   ├── planner             # Motion planner
│   │   │   │   ├── planner.cpp
│   │   │   │   └── planner.h
│   │   │   └── transition          # State transition manager
│   │   │       ├── transition.cpp
│   │   │       └── transition.h
│   │   └── web
│   │       └── index.html.gz.h     # Compressed web UI in PROGMEM
│   └── web
│       ├── README.md               # Web UI dev instructions
│       ├── gulpfile.js             # Gulp build pipeline
│       ├── package.json
│       └── src
│           ├── index.html          # Web UI source
│           └── s.js                # Dual joystick and telemetry JS
└── tools
    └── servoCalib
        └── servoCalib.ino          # Standalone servo calibration sketch
```

---

## 3. 3D Printing

### Parts List

| Part | Qty | Mirror? | Filament |
|------|-----|---------|----------|
| Body | 1 | — | PLA |
| Leg Top | 2 + 2 mirrored | Yes | PLA |
| Leg Bottom | 2 + 2 mirrored | Yes | PLA |
| Shoulder Part 1 | 4 | — | PLA |
| Shoulder Part 2 | 4 | — | PLA |
| Legs Holder Part 1 | 2 | — | PLA |
| Legs Holder Part 2 | 2 | — | PLA |
| Servo Gear | 4 | — | PLA |
| Cover | 1 | — | PLA |
| Cover Clamps | 2 | — | PLA |
| Leg Shoes | 4 | — | TPU |
| Calib Tool Beta/Gamma | 1 + 1 mirrored | Yes | PLA |
| Calib Tool Alpha | 1 | — | PLA |
| Calib Servo Disc 10° | 1 | — | PLA |

### Recommended Slicer Settings

| Setting | Value |
|---------|-------|
| Layer height | 0.12mm (Dynamic Quality) |
| Wall count | 3 |
| Infill | 30–40% |
| Infill pattern | Gyroid or Grid |
| Support | Yes — Touching Build Plate only |
| Brim | Yes — 5mm |
| Ironing | Yes — top surfaces only |
| Print speed | 40–50mm/s |
| PLA nozzle temp | 200–210°C |
| PLA bed temp | 60°C |
| TPU nozzle temp | 220–230°C |
| TPU bed temp | 40°C |

### Mirroring Parts in Your Slicer

In Cura: select part → Mirror → Mirror X.  
In PrusaSlicer: right-click part → Mirror → X axis.

### Critical Print Notes

Servo gears must be printed at maximum quality. Any dimensional error causes backlash or binding. Print at 30mm/s with ironing enabled. Elephant foot on the first layer will prevent gears from meshing — add a tiny Z-offset or enable Horizontal Expansion = -0.1mm in Cura.

Some parts have geometry that floats above the build plate — enable support for these.

After printing, test-fit bearings into the body and shoulder parts before gluing. They should press in firmly with light thumb pressure. Sand the bearing seat slightly if too tight.

Test that servo cables can thread through all routing holes before any assembly. Temporarily disassemble a servo connector to use as a probe.

---

## 4. Hardware and Electronics

### 4.1 Buck Converter Pre-Configuration

Do this before connecting any servo or the ESP32.

Power each Mini360 from the 18650 batteries (8.4V fully charged). Use a multimeter on the output pins while turning the onboard trimmer pot.

Set the converters to:
- Buck 1 → 5.0V (left-side servos)
- Buck 2 → 5.0V (right-side servos)
- Buck 3 → 3.3V (ESP32)

Verify output is stable before proceeding.

> Applying the wrong voltage will instantly destroy servos or the ESP32.

### 4.2 Prototyping Board Layout (top view suggestion)

```
[ Battery +/- IN ]        [ Power switch ]
[ Buck 1 (5V)    ]        [ Buck 2 (5V)  ]
[         Buck 3 (3.3V)                  ]
[            ESP32 (38-pin)              ]
[  INA219  ]              [ MPU9250 opt  ]
[ Servo headers — LF  RF  LH  RH        ]
```

Solder female headers for the ESP32 so it can be removed for re-flashing. Add 100µF capacitors across each 5V rail near the servo header groups. Add 100nF ceramics across the ESP32 3.3V and GND pins.

### 4.3 INA219 Connections

| INA219 Pin | Connects to |
|------------|------------|
| VCC | 3.3V |
| GND | GND |
| SDA | ESP32 GPIO 21 |
| SCL | ESP32 GPIO 22 |
| VIN+ | Battery positive (after switch) |
| VIN− | Load rail positive |

---

## 5. Wiring

### 5.1 Power Architecture

```
2× 18650 in series  →  ~7.4V nominal / 8.4V full charge
        │
        ├── [Switch] ──► Buck 1 → 5.0V → LF + LH servos (×6)
        ├── [Switch] ──► Buck 2 → 5.0V → RF + RH servos (×6)
        └── [Switch] ──► Buck 3 → 3.3V → ESP32 3V3 pin
```

All GND rails must share a common ground.

### 5.2 Servo Signal GPIO Map

Each servo has three wires: Brown = GND, Red = 5V, Orange/Yellow = Signal.

| Leg | Joint | GPIO |
|-----|-------|------|
| Left Front | Alpha — hip yaw | 25 |
| Left Front | Beta — shoulder | 26 |
| Left Front | Gamma — knee | 27 |
| Right Front | Alpha | 16 |
| Right Front | Beta | 18 |
| Right Front | Gamma | 17 |
| Left Hind | Alpha | 13 |
| Left Hind | Beta | 12 |
| Left Hind | Gamma | 14 |
| Right Hind | Alpha | 4 |
| Right Hind | Beta | 2 |
| Right Hind | Gamma | 15 |

### 5.3 I2C Bus

| Signal | GPIO |
|--------|------|
| SDA | 21 |
| SCL | 22 |

### 5.4 Pre-Power Safety Checklist

Check every item before first power-on:

- [ ] Buck 1 output confirmed 5.0V with multimeter
- [ ] Buck 2 output confirmed 5.0V with multimeter
- [ ] Buck 3 output confirmed 3.3V with multimeter
- [ ] All GND rails joined together
- [ ] No servo signal wire shorted to VCC or GND
- [ ] ESP32 seated correctly in headers, pin 1 aligned
- [ ] INA219 VCC connected to 3.3V not 5V
- [ ] No bare wire ends touching each other

---

## 6. Installing Software Tools

### 6.1 Arduino IDE

Download Arduino IDE **1.8.x** from https://www.arduino.cc/en/software

> Use Arduino IDE 1.8.x only. IDE 2.x has known issues with ESP32 multi-file sketches on some systems.

Install and launch the IDE.

### 6.2 ESP32 Board Package — v1.x ONLY

> This is the most critical setup step. ESP32 Arduino core v2.x introduced WiFi task changes that cause one core to freeze permanently during operation. This project will not work on v2.x under any circumstances. Install v1.x only.

Step by step:

1. In Arduino IDE open **File → Preferences**
2. Find the field "Additional Boards Manager URLs" and paste:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
3. Click OK to close Preferences.
4. Open **Tools → Board → Boards Manager**
5. Wait for the index to load, then search: `esp32`
6. Find the entry **"esp32 by Espressif Systems"**
7. Click the version dropdown on the right side
8. Select the **latest 1.x.x release** (for example 1.0.6)
9. Click Install and wait for it to complete

### 6.3 Board and Port Settings

Go to Tools and set each option:

| Setting | Value |
|---------|-------|
| Board | ESP32 Dev Module |
| Upload Speed | 921600 |
| Flash Size | 4MB (32Mb) |
| Partition Scheme | Default 4MB with spiffs |
| Core Debug Level | None |
| Port | your ESP32 COM port |

**Finding your COM port:**

On Windows: open Device Manager → Ports (COM & LPT). Look for "Silicon Labs CP210x" or "CH340". The port will be named COM3, COM4, etc.

On macOS: the port appears as `/dev/cu.SLAB_USBtoUART` or `/dev/cu.usbserial-XXXX`.

On Linux: look for `/dev/ttyUSB0` or `/dev/ttyACM0`. Run `ls /dev/tty*` before and after plugging in the ESP32 to spot the new entry.

If no port appears at all after plugging in the ESP32, you need to install the USB-to-UART driver. Download it from https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers — install, then unplug and replug the ESP32.

---

## 7. Installing Arduino Libraries

### 7.1 ESP32 ISR Servo — version 1.1.0 exactly

> Version must be exactly 1.1.0. Newer versions changed the API and will not compile with this firmware.

1. Open **Sketch → Include Library → Manage Libraries**
2. Search `ESP32 ISR Servo`
3. Click the version dropdown and select **1.1.0**
4. Click Install

### 7.2 MPU9250_WE — version 1.1.3 exactly

1. In Library Manager search `MPU9250_WE`
2. Select version **1.1.3** from the dropdown
3. Click Install

### 7.3 INA219_WE — latest version

1. In Library Manager search `INA219_WE`
2. Click Install (latest version is fine)

### 7.4 ESPAsyncWebServer — manual install

This library is not available in the Library Manager. Install it manually:

1. Download the ZIP: https://github.com/me-no-dev/ESPAsyncWebServer/archive/refs/heads/master.zip
2. In Arduino IDE go to **Sketch → Include Library → Add .ZIP Library**
3. Select the downloaded ZIP file and click Open

### 7.5 AsyncTCP — manual install

Required dependency for ESPAsyncWebServer:

1. Download the ZIP: https://github.com/me-no-dev/AsyncTCP/archive/refs/heads/master.zip
2. In Arduino IDE go to **Sketch → Include Library → Add .ZIP Library**
3. Select the downloaded ZIP and click Open

### 7.6 Verify Library Installation

Confirm these folders exist in your Arduino libraries directory:

- Windows: `C:\Users\YourName\Documents\Arduino\libraries\`
- macOS: `~/Documents/Arduino/libraries/`
- Linux: `~/Arduino/libraries/`

You should see folders: `ESP32_ISR_Servo`, `MPU9250_WE`, `INA219_WE`, `ESPAsyncWebServer`, `AsyncTCP`.

Check Library Manager one more time:

| Library | Must show version |
|---------|-------------------|
| ESP32 ISR Servo | 1.1.0 |
| MPU9250_WE | 1.1.3 |

---

## 8. Configuring the Firmware

### 8.1 WiFi Credentials

Navigate to `software/robot_dog_esp32/` and copy the example file:

```bash
# macOS / Linux
cp config_wifi.example.h config_wifi.h

# Windows (Command Prompt)
copy config_wifi.example.h config_wifi.h
```

Open `config_wifi.h` in any text editor and fill in your values:

```cpp
const char* APssid = "RobotDog";      // WiFi network name devices will see
const char* APpass = "yourpassword";  // Must be 8 or more characters
```

Save the file. Never commit `config_wifi.h` to version control — it is already listed in `.gitignore`.

### 8.2 Robot Geometry

Open `config_small.h` and verify the leg geometry matches your printed and assembled parts. All dimensions are in millimetres:

```cpp
#define L1  27.5   // Shoulder offset — hip centre to leg plane
#define L2  55.0   // Upper leg length — hip joint to knee joint
#define L3  55.0   // Lower leg length — knee joint to foot contact point
```

If you scaled any 3D printed part, update these values by the same scale factor.

### 8.3 Optional Hardware

In `config.h` enable or disable sensors:

```cpp
#define FEATURE_INA219   1   // Power sensor — set to 0 if not fitted
#define FEATURE_IMU      0   // MPU9250 — leave at 0, not yet implemented
```

If you did not solder an INA219, set `FEATURE_INA219` to `0`. Leaving it at `1` without the hardware will cause I2C errors on every boot.

### 8.4 Servo GPIO Map

The default GPIO assignments are already correct if you followed the wiring table in section 5.2. Only edit `config.h` if you wired servos differently:

```cpp
#define PIN_LF_ALPHA  25    // Left Front — hip yaw
#define PIN_LF_BETA   26    // Left Front — shoulder
#define PIN_LF_GAMMA  27    // Left Front — knee
// ... and so on for RF, LH, RH
```

---

## 9. Flashing the ESP32

### 9.1 Open the Sketch

In Arduino IDE: **File → Open** and navigate to:

```
software/robot_dog_esp32/robot_dog_esp32.ino
```

Arduino IDE will automatically open all `.ino` files in that folder as tabs. You should see tabs for `HAL`, `WiFi`, `gait`, `servo`, `cli`, etc. This is expected and correct.

### 9.2 Compile First

Before uploading, verify the code compiles cleanly:

**Sketch → Verify/Compile** (or press Ctrl+R)

A successful compile ends with a line similar to:
```
Sketch uses 842736 bytes (64%) of program storage space.
```

If there are errors, check the Troubleshooting section before proceeding.

### 9.3 Upload to the ESP32

1. Connect the ESP32 to your PC via USB.
2. Confirm the correct port is selected under Tools → Port.
3. Hold the **BOOT** button on the ESP32 board.
4. Click **Upload** in Arduino IDE (Ctrl+U).
5. Watch the console at the bottom. When you see:
   ```
   Connecting........_____
   ```
   Release the BOOT button.
6. Wait for the upload to finish. Success message:
   ```
   Hard resetting via RTS pin...
   ```

> Some ESP32 boards upload automatically without needing the BOOT button. If it works without holding it, that is fine.

### 9.4 Verify Boot via Serial Monitor

1. Open **Tools → Serial Monitor** (Ctrl+Shift+M)
2. Set the baud rate dropdown to **115200**
3. Press the **EN** (Enable/Reset) button on the ESP32 board
4. You should see output similar to:

```
── ESP32 Robot Dog ──
HAL ESP32_ISR v1.1.0
AP: RobotDog  IP: 192.168.4.1
Ready. Type 'set help' for CLI commands.
```

If you see random symbols or garbled text, the baud rate is wrong — set it to 115200. If nothing appears at all, the upload failed — retry section 9.3.

---

## 10. Servo Calibration

Do this before assembling the robot mechanically. Calibrate servos individually to record the exact pulse-width limits for your specific servo batch.

### 10.1 Flash the Calibration Sketch

1. In Arduino IDE open `tools/servoCalib/servoCalib.ino`
2. Connect one servo signal wire to **GPIO 14** on the ESP32
3. Power the servo from your pre-configured 5V buck converter (not from the ESP32 itself)
4. Upload the calibration sketch to the ESP32

### 10.2 Find Pulse Width Limits

Open Serial Monitor at 115200 baud. Type values in microseconds and press Enter.

Start with the centre position:
```
1500
```
The servo should move to its mechanical centre. Confirm visually.

**Finding minimum (minUs):**

Start at 800 and decrease in steps of 10 until the servo stops moving, then step back one value:
```
800  →  servo moves
770  →  servo moves
750  →  servo does not move  ← stall point
760  →  use this as minUs
```

**Finding maximum (maxUs):**

Start at 2200 and increase in steps of 10 until the servo stops, then step back:
```
2200  →  servo moves
2240  →  servo moves
2260  →  servo does not move  ← stall point
2250  →  use this as maxUs
```

**Finding degree reference points** using the printed 10° calibration disc:

Mount the disc on the servo horn and send values until the horn aligns with each marked angle:
```
Find µs value for 30°, 60°, 90°, 120°, 150°
The 90° value should be close to 1500µs
```

### 10.3 Record Your Values

| Servo | Index | minUs | maxUs | µs at 90° |
|-------|-------|-------|-------|-----------|
| LF Alpha | 0 | | | |
| LF Beta  | 1 | | | |
| LF Gamma | 2 | | | |
| RF Alpha | 3 | | | |
| RF Beta  | 4 | | | |
| RF Gamma | 5 | | | |
| LH Alpha | 6 | | | |
| LH Beta  | 7 | | | |
| LH Gamma | 8 | | | |
| RH Alpha | 9 | | | |
| RH Beta  | 10 | | | |
| RH Gamma | 11 | | | |

Servos from the same production batch usually have very similar limits. Measuring 2–3 representative samples and using the average is acceptable.

### 10.4 Save Calibration to Firmware

Open `config_small.h` and update each servo profile entry with your recorded minUs, maxUs, and degree mapping. Then reflash the main sketch (section 9.3) before continuing to assembly.

---

## 11. Mechanical Assembly

Work through these steps in order. Do not skip ahead.

### 11.1 Prepare All Printed Parts

Use a flat needle file on every surface that will be glued. Parts must mate flush with zero gap. Do a dry-fit for every joint before applying glue. Test-fit all 8 bearings into their seats. Thread a servo cable through every routing channel to confirm clearance.

### 11.2 Glue Body Sub-Assemblies

Apply cyanoacrylate in a thin bead. Press and hold for 30 seconds. Allow 5 minutes cure time before handling.

Glue these pairs:
- Legs Holder Part 1 + Part 2 (two pairs total)
- Shoulder Part 1 + Part 2 (four pairs total)

Keep glue away from bearing seats and servo screw holes.

### 11.3 Servo Gears

For each of the 4 shoulder servo gears:

1. Take a servo horn and cut all arms flush with the hub using wire cutters.
2. Apply a thin ring of super glue inside the servo gear recess.
3. Press the trimmed horn hub into the gear. Align the flat of the hub before the glue sets.
4. Set aside to cure. Do not install on servo shafts yet.

### 11.4 Leg Sub-Assemblies (repeat for all 4 legs)

1. Glue a short servo horn into the recess in the Leg Top part.
2. Insert the knee servo into the Leg Bottom part and route its cable out through the bottom opening.
3. Thread the shoulder servo cable through the routing hole in the Shoulder part.

### 11.5 Assemble Body and Legs

1. Thread all 12 servo cables through the leg channels and up into the body cavity.

2. Before installing any servo gear, set all 12 servos to exactly 90°. Flash `tools/servoCalib/servoCalib.ino`, connect each servo in turn to GPIO 14, send `1500`, confirm the servo is centred, then disconnect. Repeat for all 12.

3. Install the 4 shoulder servos into the body and screw them in place.

4. Install each servo gear onto its shaft at the centred (90°) position.

5. Squeeze each Shoulder assembly (with cable threaded, no servo) and seat it between the bearings in the body frame.

6. Do not tighten leg bolts permanently yet. Leave them hand-tight for the calibration step.

### 11.6 Solder and Install Electronics

1. Mount the proto board inside or on top of the body.
2. Plug all servo connectors to their corresponding GPIO headers. Double-check every servo against the wiring table in section 5.2.
3. Connect INA219 to I2C headers and 3.3V power.
4. Seat the ESP32 in its socket.
5. Run the main power wires from batteries through the switch to the buck converters.

---

## 12. Leg Calibration

With the robot assembled and electronics wired, fine-tune each joint using the Serial CLI and the printed calibration tools.

### 12.1 Set All Servos to Calibration Position

Power the robot from USB only during calibration (safer, no risk of the robot falling).

Open Serial Monitor at 115200 baud and send:

```
set servo_to_calib
```

All servos will move to the reference calibration position: Alpha 90°, Beta 45°, Gamma 90°.

### 12.2 Use the Printed Calibration Templates

Place the Beta/Gamma calibration template against each leg. The flat reference face of the template should sit perfectly flush against the servo body surface.

Place the Alpha calibration template on top of the body against the shoulder servo. Its flat reference should align with the servo body.

If a surface is not flush, that joint needs a trim correction.

### 12.3 Apply Trim Corrections via CLI

```bash
# Syntax
set XX_HAL_trim_JOINT value

# Examples
set LF_HAL_trim_alpha -3
set LF_HAL_trim_beta   2
set RF_HAL_trim_gamma -1
```

Leg prefixes: `LF` `RF` `LH` `RH`  
Joint names: `alpha` `beta` `gamma`

After each set command the servo will move to the trimmed position immediately. Re-check with the template. Iterate until flush.

Keep trim values under ±10°. If a larger offset is needed, the physical assembly of that leg is too far off and needs to be redone.

### 12.4 Save Calibration Permanently

Trim values held in RAM are lost on power cycle. Once all joints are dialled in, open `config_small.h` and write the trim values into each servo profile. Reflash the firmware. Then tighten all leg bolts to their final position.

---

## 13. First Boot and Web Control

### 13.1 Power On

Confirm all items in the wiring safety checklist (section 5.4) are checked. Flip the main power switch. The robot will initialise and move to its standing position within about 2 seconds.

### 13.2 Connect to the Web Interface

1. On your phone, tablet, or computer open WiFi settings.
2. Connect to the network: **RobotDog** (or your configured AP name).
3. Enter the password you set in `config_wifi.h`.
4. Open a browser and go to: `http://192.168.4.1`
5. The dual-joystick control page will load.

The interface works on any modern browser. Chrome and Safari on mobile are recommended.

### 13.3 Controls

| Control | Action |
|---------|--------|
| Left joystick — push forward/back | Walk forward / backward |
| Left joystick — push left/right | Strafe left / right |
| Right joystick — push left/right | Yaw (rotate in place) |
| Both joysticks at centre | Robot stands still |

### 13.4 Battery Telemetry

If INA219 is fitted, the web page shows live battery voltage and current draw.

- Full charge voltage: ~8.4V
- Recommended low-voltage cutoff: 7.2V
- Hard cutoff per cell: 3.5V (7.0V total)

Stop the robot and recharge when voltage drops below 7.2V under load.

---

## 14. Serial CLI Reference

Open Serial Monitor at 115200 baud at any time while the ESP32 is connected via USB.

### Get Commands

```bash
get help          # list all available get commands
get telemetry     # print battery voltage, current, uptime
get settings      # print all current runtime settings
get servos        # print servo positions and active trims
```

### Set Commands

```bash
set help                       # list all set commands

set servo_to_calib             # move all servos to calibration position

set LF_HAL_trim_alpha <deg>    # fine-tune left-front hip yaw
set LF_HAL_trim_beta  <deg>    # fine-tune left-front shoulder
set LF_HAL_trim_gamma <deg>    # fine-tune left-front knee
# RF, LH, RH follow the same pattern

set body_height <mm>           # adjust standing body height
set gait_speed  <ms>           # gait cycle period in milliseconds
set step_height <mm>           # foot lift height per step
```

### I2C Scanner

```bash
i2cscan
```

Scans the I2C bus and prints all found device addresses. Expected results:

| Address | Device |
|---------|--------|
| 0x40 | INA219 |
| 0x68 | MPU9250 (if fitted) |

If INA219 is not found, check SDA/SCL wiring and confirm VCC is 3.3V not 5V.

---

## 15. Rebuilding the Web UI

Only needed if you modify the web interface source files in `software/web/src/`.

### 15.1 Prerequisites

Install Node.js v16 or later from https://nodejs.org

Verify installation:
```bash
node --version   # should print v16.x.x or higher
npm --version
```

### 15.2 Install Build Dependencies

```bash
cd software/web
npm install
```

### 15.3 Build

```bash
npm run build
```

The Gulp pipeline will:
1. Minify `index.html` and `s.js`
2. GZip-compress the result
3. Convert it to a C header array `index.html.gz.h`
4. Copy the header to `software/robot_dog_esp32/web/`

### 15.4 Reflash

After the build completes, reflash the main sketch following section 9.3. The new web UI is now embedded in the firmware.

---

## 16. Troubleshooting

### No COM port appears when ESP32 is plugged in

Install the CP210x USB-to-UART driver from https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers — unplug and replug after installing. If your board uses a CH340 chip instead, install the CH340 driver. Try a different USB cable — many cables are charge-only and carry no data.

On Linux add your user to the dialout group and log out and back in:
```bash
sudo usermod -aG dialout $USER
```

### Upload fails with "Connecting........______"

Hold the BOOT button on the ESP32 before clicking Upload and release it only after `Connecting...` appears. Some boards require this.

### Compilation fails — library not found

Confirm library folder names match exactly. Arduino is case-sensitive on macOS and Linux. The ESP32 ISR Servo folder must be named `ESP32_ISR_Servo`. Re-install from Library Manager if unsure.

### Compilation fails — version mismatch errors

You likely have ESP32 core v2.x installed. Go to Boards Manager, find "esp32 by Espressif Systems", change version to the latest 1.x.x, and reinstall.

### Serial Monitor shows only garbled symbols

The baud rate dropdown in Serial Monitor must be set to **115200**. The sketch does not use the default 9600.

### WiFi AP appears but `http://192.168.4.1` does not load

Make sure your device is connected to the robot's AP and not your home router — they cannot both be active on a phone at the same time. Navigate to exactly `http://192.168.4.1` using http, not https. If a browser auto-upgrades to https, try a different browser or clear browser cache.

### Servos make noise but legs do not move to correct positions

All 12 servos must have been centred at 1500µs before the servo gears were installed. If they were not, disassemble, re-centre, and reinstall. Double-check every servo is plugged into the correct GPIO per the table in section 5.2.

### One leg behaves incorrectly while the others are fine

A servo signal wire is connected to the wrong GPIO. Recheck that leg's three connections against the wiring table. Alternatively the inverted flag for that servo may be wrong in `config_small.h`.

### Robot falls to one side immediately on standing

The leg calibration trims for that side are off. Redo section 12 for the affected legs. Also check that the correct mirrored versions of Leg Top and Leg Bottom were used.

### Battery drains within minutes

One or more servos are stalled or fighting a mechanical bind. Listen for a buzzing servo. Identify it by feeling for heat on each servo case. Fix the mechanical bind causing the stall. Idle current while standing should be approximately 400–600mA. Walking peak current is 1.5–2.5A. Anything higher indicates a problem.

### INA219 not found on I2C scan

Check that VCC is connected to 3.3V and not 5V. Confirm SDA is on GPIO 21 and SCL is on GPIO 22. Run `i2cscan` in the Serial CLI to check what addresses are visible on the bus.

---

## License

MIT — see [LICENSE](LICENSE).