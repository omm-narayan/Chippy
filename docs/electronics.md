# Electronics & Wiring

## Power Architecture

```
[2× 18650 in series ~8.4V]
        │
        ├──► Mini360 #1 → 5V ──► Left-side servos (×6)
        ├──► Mini360 #2 → 5V ──► Right-side servos (×6)
        └──► Mini360 #3 → 3.3V ──► ESP32 (3V3 pin)
```

> ⚠️ Never power servos from the ESP32's 3V3 or 5V pins — they cannot supply enough current.

---

## Servo Wiring

Each servo has 3 wires: **GND (Brown) · VCC (Red) · Signal (Orange/Yellow)**

- GND → Common ground rail
- VCC → 5V output of corresponding Buck converter
- Signal → ESP32 GPIO (see table below)

| Leg | Joint | GPIO |
|-----|-------|------|
| Left Front  | Alpha (body rotate) | 25 |
| Left Front  | Beta (shoulder)     | 26 |
| Left Front  | Gamma (knee)        | 27 |
| Right Front | Alpha               | 16 |
| Right Front | Beta                | 18 |
| Right Front | Gamma               | 17 |
| Left Hind   | Alpha               | 13 |
| Left Hind   | Beta                | 12 |
| Left Hind   | Gamma               | 14 |
| Right Hind  | Alpha               |  4 |
| Right Hind  | Beta                |  2 |
| Right Hind  | Gamma               | 15 |

---

## I²C Sensors

| Sensor | SDA | SCL | Address |
|--------|-----|-----|---------|
| INA219 | 21  | 22  | 0x40    |
| MPU9250 (optional) | 21 | 22 | 0x68 |

---

## Decoupling Capacitors

Place 100µF electrolytic capacitors across the 5V rail near servo clusters.  
Add 100nF ceramics near ESP32 power pins.

---

## Schematic

Full schematic: [`hardware/schematics/wiring.pdf`](../hardware/schematics/wiring.pdf)

---

## Assembly Notes

- The prototyping board (50×70mm green) is used as a manual PCB
- Hot-glue headers in place before soldering for alignment
- Keep signal wires away from power wires to minimize noise on servo PWM lines
- IMU should mount as flat and centred on the body as possible
