#pragma once
#include "config.h"
#include "kinematics.h"
#include "servo_hal.h"

// ─────────────────────────────────────────────
//  Gait Sequencer — Trot & Creep gaits
// ─────────────────────────────────────────────

enum GaitType {
    GAIT_STAND = 0,
    GAIT_TROT,
    GAIT_CREEP,
};

struct GaitConfig {
    GaitType type        = GAIT_STAND;
    float    stepHeight  = GAIT_STEP_HEIGHT_MM;
    float    stepLength  = GAIT_STEP_LENGTH_MM;
    int      periodMs    = GAIT_SPEED_MS;
    float    bodyHeight  = 90.0f;   // mm from ground
    float    vx          = 0.0f;    // forward velocity (-1..1)
    float    vy          = 0.0f;    // lateral velocity (-1..1)
    float    wz          = 0.0f;    // yaw rate (-1..1)
};

// Default foot positions (relative to hip) at stand
static Vec3 defaultFeet[4] = {
    {  60.0f, -70.0f, -90.0f },   // LF
    {  60.0f,  70.0f, -90.0f },   // RF
    { -60.0f, -70.0f, -90.0f },   // LH
    { -60.0f,  70.0f, -90.0f },   // RH
};

// Trot phase offsets for each leg (0.0 – 1.0)
static float trotPhase[4] = { 0.0f, 0.5f, 0.5f, 0.0f };

static unsigned long gaitStartMs = 0;
static GaitConfig    currentGait;

inline void gait_setConfig(const GaitConfig& cfg) {
    currentGait = cfg;
    gaitStartMs = millis();
}

inline void gait_stand(float bodyHeight) {
    for (int leg = 0; leg < 4; leg++) {
        Vec3 target = defaultFeet[leg];
        target.z = -bodyHeight;
        bool isRight = (leg == 1 || leg == 3);
        bool ok;
        LegAngles angles = ik_solve(target, isRight, &ok);
        if (ok) {
            int base = leg * 3;
            servoHAL_write(base + 0, 90.0f + angles.alpha);
            servoHAL_write(base + 1, 90.0f + angles.beta);
            servoHAL_write(base + 2, 90.0f + angles.gamma);
        }
    }
}

inline void gait_update() {
    if (currentGait.type == GAIT_STAND || 
        (fabsf(currentGait.vx) < 0.01f && fabsf(currentGait.vy) < 0.01f && fabsf(currentGait.wz) < 0.01f)) {
        gait_stand(currentGait.bodyHeight);
        return;
    }

    float t = (float)((millis() - gaitStartMs) % currentGait.periodMs) / (float)currentGait.periodMs;

    for (int leg = 0; leg < 4; leg++) {
        float phase = fmodf(t + trotPhase[leg], 1.0f);
        Vec3 foot   = defaultFeet[leg];

        float dx = currentGait.stepLength * currentGait.vx;
        float dy = currentGait.stepLength * currentGait.vy;

        if (phase < 0.5f) {
            // Swing phase
            float s   = phase / 0.5f;
            float swx = (s - 0.5f) * dx;
            float swy = (s - 0.5f) * dy;
            float swz = sinf(s * M_PI) * currentGait.stepHeight;
            foot.x += swx;
            foot.y += swy;
            foot.z = -currentGait.bodyHeight + swz;
        } else {
            // Stance phase
            float s   = (phase - 0.5f) / 0.5f;
            foot.x += (0.5f - s) * dx;
            foot.y += (0.5f - s) * dy;
            foot.z = -currentGait.bodyHeight;
        }

        bool isRight = (leg == 1 || leg == 3);
        bool ok;
        LegAngles angles = ik_solve(foot, isRight, &ok);
        if (ok) {
            int base = leg * 3;
            servoHAL_write(base + 0, 90.0f + angles.alpha);
            servoHAL_write(base + 1, 90.0f + angles.beta);
            servoHAL_write(base + 2, 90.0f + angles.gamma);
        }
    }
}
