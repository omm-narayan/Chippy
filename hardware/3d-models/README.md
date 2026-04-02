# 3D Models

All STL files are organized by sub-assembly.  
Fusion 360 source files (`.f3d`) are included where applicable.

## Folder Layout

```
3d-models/
├── body/
│   ├── body.stl
│   └── body.f3d
├── legs/
│   ├── leg_top.stl
│   ├── leg_top_mirror.stl
│   ├── leg_bottom.stl
│   ├── leg_bottom_mirror.stl
│   └── leg_shoe_tpu.stl          ← flexible filament
├── shoulder/
│   ├── shoulder_p1.stl
│   ├── shoulder_p2.stl
│   ├── legs_holder_p1.stl
│   └── legs_holder_p2.stl
├── assembly/
│   ├── servo_gear.stl
│   ├── cover.stl
│   └── cover_clamp.stl
└── calibration-tools/
    ├── calib_beta_gamma.stl
    ├── calib_beta_gamma_mirror.stl
    ├── calib_alpha.stl
    └── calib_servo_10deg.stl
```

## Print Settings

| Setting | Value |
|---------|-------|
| Layer height | 0.12 mm (Dynamic Quality) |
| Infill | 30–40% |
| Support | Yes (Touching Build Plate) |
| Brim | Yes (5 mm) |
| Ironing | Yes (top surfaces) |
| Material | PLA (structural), TPU (shoes) |

## Mirror Parts

Parts marked `_mirror` must be printed mirrored.  
In your slicer: **Mirror on X axis** before slicing.

| Part | Mirror needed |
|------|--------------|
| leg_top | ✅ |
| leg_bottom | ✅ |
| calib_beta_gamma | ✅ |

## Renders & Animation

Fusion 360 rendered views and assembly animation are in [`../../assets/renders/`](../../assets/renders/).
