#pragma once
#include <ESP32_ISR_Servo.h>
#include "config.h"

// ─────────────────────────────────────────────
//  Servo Hardware Abstraction Layer
// ─────────────────────────────────────────────

struct ServoProfile {
    int pin;
    int minUs;
    int maxUs;
    int degMin;
    int degMax;
    float trim;       // Calibration offset (degrees)
    bool  inverted;
};

// 12 servos: LF[0..2], RF[3..5], LH[6..8], RH[9..11]
// Index mapping: 0=alpha, 1=beta, 2=gamma per leg block
static ServoProfile servoProfiles[12] = {
    // Left Front
    { PIN_LF_ALPHA, SERVO_MIN_US, SERVO_MAX_US,  0, 180, 0.0f, false },
    { PIN_LF_BETA,  SERVO_MIN_US, SERVO_MAX_US,  0, 180, 0.0f, false },
    { PIN_LF_GAMMA, SERVO_MIN_US, SERVO_MAX_US,  0, 180, 0.0f, false },
    // Right Front
    { PIN_RF_ALPHA, SERVO_MIN_US, SERVO_MAX_US,  0, 180, 0.0f, true  },
    { PIN_RF_BETA,  SERVO_MIN_US, SERVO_MAX_US,  0, 180, 0.0f, true  },
    { PIN_RF_GAMMA, SERVO_MIN_US, SERVO_MAX_US,  0, 180, 0.0f, true  },
    // Left Hind
    { PIN_LH_ALPHA, SERVO_MIN_US, SERVO_MAX_US,  0, 180, 0.0f, false },
    { PIN_LH_BETA,  SERVO_MIN_US, SERVO_MAX_US,  0, 180, 0.0f, false },
    { PIN_LH_GAMMA, SERVO_MIN_US, SERVO_MAX_US,  0, 180, 0.0f, false },
    // Right Hind
    { PIN_RH_ALPHA, SERVO_MIN_US, SERVO_MAX_US,  0, 180, 0.0f, true  },
    { PIN_RH_BETA,  SERVO_MIN_US, SERVO_MAX_US,  0, 180, 0.0f, true  },
    { PIN_RH_GAMMA, SERVO_MIN_US, SERVO_MAX_US,  0, 180, 0.0f, true  },
};

static int servoIdx[12];

// ─────────────────────────────────────────────
//  Init all 12 servos
// ─────────────────────────────────────────────
inline void servoHAL_init() {
    ESP32_ISR_Servos.useTimer(USE_ESP32_TIMER_NO);
    for (int i = 0; i < 12; i++) {
        ServoProfile& p = servoProfiles[i];
        servoIdx[i] = ESP32_ISR_Servos.setupServo(p.pin, p.minUs, p.maxUs);
    }
}

// ─────────────────────────────────────────────
//  Write angle (degrees) to servo index i
// ─────────────────────────────────────────────
inline void servoHAL_write(int i, float angleDeg) {
    ServoProfile& p = servoProfiles[i];
    float a = angleDeg + p.trim;
    if (p.inverted) a = (float)(p.degMax + p.degMin) - a;
    a = constrain(a, (float)p.degMin, (float)p.degMax);
    ESP32_ISR_Servos.setPosition(servoIdx[i], (int)a);
}

// ─────────────────────────────────────────────
//  Set trim (degrees) for a servo index
// ─────────────────────────────────────────────
inline void servoHAL_setTrim(int i, float trim) {
    servoProfiles[i].trim = trim;
}

// ─────────────────────────────────────────────
//  Move all servos to mid position (calibration)
// ─────────────────────────────────────────────
inline void servoHAL_centerAll() {
    for (int i = 0; i < 12; i++) {
        servoHAL_write(i, 90.0f);
    }
}
