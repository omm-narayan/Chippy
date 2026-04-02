#pragma once
#include <math.h>
#include "config.h"

// ─────────────────────────────────────────────
//  3-DOF Leg Inverse Kinematics
//  Coordinate system: X = forward, Y = lateral, Z = up
// ─────────────────────────────────────────────

struct LegAngles {
    float alpha;  // Hip rotate  (degrees)
    float beta;   // Hip swing   (degrees)
    float gamma;  // Knee        (degrees)
};

struct Vec3 {
    float x, y, z;
};

// Clamp helper
inline float clampf(float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

// Returns joint angles (degrees) for a target foot position.
// Returns {0,0,0} and sets *ok=false on unreachable target.
inline LegAngles ik_solve(Vec3 target, bool isRight, bool* ok = nullptr) {
    LegAngles out = {0, 0, 0};

    float x = target.x;
    float y = isRight ? -target.y : target.y;   // Mirror for right legs
    float z = target.z;

    // Alpha: hip yaw (lateral offset)
    float alpha_rad = atan2f(y, fabsf(z) + 1e-6f);
    out.alpha = alpha_rad * 180.0f / M_PI;

    // Project onto sagittal plane
    float L = sqrtf(y * y + z * z) - L1;
    float D = sqrtf(x * x + L * L);

    if (D > (L2 + L3) * 0.99f) {
        if (ok) *ok = false;
        return out;
    }

    // Beta: hip pitch
    float a1 = atan2f(x, -L);
    float a2 = acosf(clampf((L2 * L2 + D * D - L3 * L3) / (2.0f * L2 * D), -1.0f, 1.0f));
    out.beta = (a1 + a2) * 180.0f / M_PI;

    // Gamma: knee
    float a3 = acosf(clampf((L2 * L2 + L3 * L3 - D * D) / (2.0f * L2 * L3), -1.0f, 1.0f));
    out.gamma = (a3 - M_PI) * 180.0f / M_PI;

    if (ok) *ok = true;
    return out;
}
