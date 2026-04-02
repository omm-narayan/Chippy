# Assembly Guide

## Before You Start

- Check all bearings fit snugly into body and over shoulder parts
- Test that servo cables fit through all routing holes **before** final assembly
- Print servo gears at maximum quality — they take the most mechanical stress

---

## Step 1 — Prepare Parts

1. Use a flat mini file to ensure all gluing surfaces are flush
2. Do a dry-fit before applying glue — parts should mate without gaps

---

## Step 2 — Glue Body Sub-Assemblies

Apply cyanoacrylate (super glue) sparingly:

- Join **Legs Holder Part 1 + Part 2** pairs
- Join **Shoulder Part 1 + Part 2** pairs
- Allow each joint to fully cure before handling

> ℹ️ Keep glue away from bearing seats and servo mounting holes.

---

## Step 3 — Servo Gears

1. Cut servo horn arms flush so the hub fits inside the servo gear cavity
2. Apply a small bead of super glue inside the gear
3. Press horn hub into gear — align orientation before glue sets
4. Install gear on servo shaft as normal

---

## Step 4 — Leg Sub-Assemblies

For each leg (×4):

1. Glue short servo horns into the **Leg Top** parts
2. Insert servo into **Leg Bottom** part; route cable as shown in [`docs/images/leg-wiring.png`](images/leg-wiring.png)
3. Pull second servo cable through the shoulder routing hole

---

## Step 5 — Install Legs into Body

1. Thread all servo cables through leg channels and into body cavity
2. **Set all 12 servos to 90° (1500µs) before installing gears**  
   Use the servo calibration tool or any servo tester
3. Install shoulder servos into body — hold in place, then screw
4. Squeeze each **Shoulder** part (without servo, cable in place) and seat between bearings
5. Repeat for all four legs

> Do not bolt legs permanently at this stage — leave them loose for calibration.

---

## Step 6 — Electronics

See [`docs/electronics.md`](electronics.md) for full wiring detail.

Short version:

- Wire each servo to its GPIO as defined in `config.h`
- Connect INA219 to I²C (SDA=21, SCL=22)
- Set Buck converters: 5V for servos, 3.3V for ESP32

---

## Step 7 — Flash & Calibrate

1. Flash firmware (see main README)
2. Follow [`docs/calibration.md`](calibration.md)
3. Bolt legs once calibration is complete

---

## Step 8 — Fit Cover & Clamps

Snap cover clamps into body slots and press cover into place.  
Route antenna away from servo cables if possible.
