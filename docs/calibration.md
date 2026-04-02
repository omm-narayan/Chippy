# Calibration Guide

## 1 — Servo Profile Calibration

Use the standalone sketch at `firmware/tools/servo_calibration/servo_calib.ino`.

Connect **one servo** to GPIO 14. Open Serial Monitor at 115200 baud.

```
1500    ← center (servo should point to middle dot on printed disc)
780     ← find minimum (decrease until servo stalls, step back one value)
2200    ← find maximum (increase until servo stalls, step back)
```

Record `minUs`, `maxUs`, `degMin`, `degMax` for each servo batch and update `config.h`.

---

## 2 — Leg Assembly Position

Print the calibration tools from `hardware/3d-models/calibration-tools/`:

| Tool | Purpose |
|------|---------|
| `CALIB_BETA_GAMMA.stl` | Verify Beta + Gamma angles |
| `CALIB_BETA_GAMMA_mirror.stl` | Mirror — opposite side legs |
| `CALIB_ALPHA.stl` | Verify Alpha angle |

**Procedure:**

1. Flash firmware and open Arduino IDE Serial Monitor (115200 baud)
2. Send: `set servo_to_calib`  
   All servos move to calibration position (90°/45°/90° for α/β/γ)
3. Assemble each leg so its physical angle **matches the printed template**

---

## 3 — Fine Trim Calibration

After assembly, fine-tune each joint:

```bash
# List all trim commands
set help

# Examples
set LF_HAL_trim_alpha -3
set RF_HAL_trim_beta   2
set LH_HAL_trim_gamma  1
```

**Leg prefixes:** `LF` `RF` `LH` `RH`  
**Joint names:** `alpha` · `beta` · `gamma`

Place the calibration template against each joint surface. Adjust trim until the surface aligns with the template reference plane.

> ⚠️ Keep trim values small (< ±10°). Large offsets mean the physical assembly needs to be redone.

---

## 4 — Save Calibration

Trim values are stored in RAM and lost on reboot.  
Persist them by editing `servoProfiles[].trim` values in `servo_hal.h` once you're satisfied.

---

## Calibration Reference Table (fill in your values)

| Servo | Index | minUs | maxUs | trim |
|-------|-------|-------|-------|------|
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
| RH Beta  |10 | | | |
| RH Gamma |11 | | | |
