#pragma once

// ─────────────────────────────────────────────
//  GPIO — Servo Pin Map
//  Alpha = body rotate | Beta = shoulder | Gamma = knee
// ─────────────────────────────────────────────

// Left Front
#define PIN_LF_ALPHA  25
#define PIN_LF_BETA   26
#define PIN_LF_GAMMA  27

// Right Front
#define PIN_RF_ALPHA  16
#define PIN_RF_BETA   18
#define PIN_RF_GAMMA  17

// Left Hind
#define PIN_LH_ALPHA  13
#define PIN_LH_BETA   12
#define PIN_LH_GAMMA  14

// Right Hind
#define PIN_RH_ALPHA   4
#define PIN_RH_BETA    2
#define PIN_RH_GAMMA  15

// ─────────────────────────────────────────────
//  I²C Bus (INA219 + IMU)
// ─────────────────────────────────────────────
#define PIN_I2C_SDA   21
#define PIN_I2C_SCL   22

// ─────────────────────────────────────────────
//  Servo Pulse Limits (µs)
// ─────────────────────────────────────────────
#define SERVO_MIN_US   500
#define SERVO_MID_US  1500
#define SERVO_MAX_US  2500

// ─────────────────────────────────────────────
//  Leg Geometry (mm)
// ─────────────────────────────────────────────
#define L1  27.5f   // Shoulder offset
#define L2  55.0f   // Upper leg length
#define L3  55.0f   // Lower leg length

// ─────────────────────────────────────────────
//  Gait Defaults
// ─────────────────────────────────────────────
#define GAIT_STEP_HEIGHT_MM   20.0f
#define GAIT_STEP_LENGTH_MM   30.0f
#define GAIT_SPEED_MS         300

// ─────────────────────────────────────────────
//  Features
// ─────────────────────────────────────────────
#define FEATURE_INA219   1   // 1 = enabled, 0 = disabled
#define FEATURE_IMU      0   // MPU9250 (WIP)

// ─────────────────────────────────────────────
//  Web Server
// ─────────────────────────────────────────────
#define WEB_SERVER_PORT  80
