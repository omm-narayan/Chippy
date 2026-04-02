// ─────────────────────────────────────────────
//  Servo Calibration Tool
//  Connect ONE servo to GPIO 14.
//  Open Serial Monitor @ 115200.
//  Send pulse width in µs (e.g. 1500) to set position.
// ─────────────────────────────────────────────

#include <ESP32_ISR_Servo.h>

#define SERVO_PIN     14
#define SERVO_MIN_US  500
#define SERVO_MAX_US  2500

int servoIdx = -1;

void setup() {
    Serial.begin(115200);
    Serial.println("── Servo Calibration Tool ──");
    Serial.println("Send µs value (500–2500) and press Enter.");
    Serial.println("Middle = 1500");

    ESP32_ISR_Servos.useTimer(USE_ESP32_TIMER_NO);
    servoIdx = ESP32_ISR_Servos.setupServo(SERVO_PIN, SERVO_MIN_US, SERVO_MAX_US);

    if (servoIdx < 0) {
        Serial.println("ERROR: Servo init failed.");
    } else {
        ESP32_ISR_Servos.setPulseWidth(servoIdx, 1500);
        Serial.println("Servo centred at 1500µs.");
    }
}

void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        if (input.length() == 0) return;

        int us = input.toInt();
        if (us < SERVO_MIN_US || us > SERVO_MAX_US) {
            Serial.printf("Out of range. Valid: %d – %d µs\n", SERVO_MIN_US, SERVO_MAX_US);
            return;
        }

        ESP32_ISR_Servos.setPulseWidth(servoIdx, us);
        Serial.printf("Set: %d µs\n", us);
    }
}
