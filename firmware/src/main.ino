// ─────────────────────────────────────────────
//  ESP32 Robot Dog — Main Sketch
//  12-DOF Quadruped | WiFi Web Control | IK Gait
// ─────────────────────────────────────────────

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "config.h"
#include "config_wifi.h"
#include "servo_hal.h"
#include "kinematics.h"
#include "gait.h"

#if FEATURE_INA219
#include <INA219_WE.h>
INA219_WE ina219;
float batteryVoltage = 0.0f;
float batteryCurrent = 0.0f;
#endif

AsyncWebServer server(WEB_SERVER_PORT);
TaskHandle_t gaitTaskHandle = NULL;

// ─────────────────────────────────────────────
//  Gait loop — runs on Core 0
// ─────────────────────────────────────────────
void gaitTask(void* pvParams) {
    for (;;) {
        gait_update();
        vTaskDelay(pdMS_TO_TICKS(20));   // ~50Hz update
    }
}

// ─────────────────────────────────────────────
//  Serial CLI
// ─────────────────────────────────────────────
void handleCLI(const String& cmd) {
    if (cmd == "set help") {
        Serial.println("Commands:");
        Serial.println("  set servo_to_calib          — center all servos");
        Serial.println("  set LF_HAL_trim_alpha <val> — trim left-front alpha");
        Serial.println("  set LF_HAL_trim_beta  <val>");
        Serial.println("  set LF_HAL_trim_gamma <val>");
        Serial.println("  (RF / LH / RH likewise)");
        return;
    }
    if (cmd == "set servo_to_calib") {
        servoHAL_centerAll();
        Serial.println("All servos centred.");
        return;
    }

    // Parse: set XX_HAL_trim_YYY value
    // e.g.  set LF_HAL_trim_alpha -3
    if (cmd.startsWith("set ") && cmd.indexOf("_HAL_trim_") > 0) {
        int sp = cmd.lastIndexOf(' ');
        String token = cmd.substring(4, sp);     // e.g. LF_HAL_trim_alpha
        float val    = cmd.substring(sp + 1).toFloat();

        const char* legs[]   = {"LF", "RF", "LH", "RH"};
        const char* joints[] = {"alpha", "beta", "gamma"};

        for (int l = 0; l < 4; l++) {
            for (int j = 0; j < 3; j++) {
                String expected = String(legs[l]) + "_HAL_trim_" + String(joints[j]);
                if (token == expected) {
                    servoHAL_setTrim(l * 3 + j, val);
                    Serial.printf("Trim %s set to %.1f°\n", expected.c_str(), val);
                    return;
                }
            }
        }
    }
    Serial.println("Unknown command. Type 'set help'.");
}

// ─────────────────────────────────────────────
//  Web API handlers
// ─────────────────────────────────────────────
void setupRoutes() {
    // Control joystick input: /ctrl?vx=0.5&vy=0&wz=0
    server.on("/ctrl", HTTP_GET, [](AsyncWebServerRequest* req) {
        GaitConfig cfg = currentGait;
        if (req->hasParam("vx"))  cfg.vx  = req->getParam("vx")->value().toFloat();
        if (req->hasParam("vy"))  cfg.vy  = req->getParam("vy")->value().toFloat();
        if (req->hasParam("wz"))  cfg.wz  = req->getParam("wz")->value().toFloat();
        cfg.type = (fabsf(cfg.vx) > 0.05f || fabsf(cfg.vy) > 0.05f || fabsf(cfg.wz) > 0.05f)
                   ? GAIT_TROT : GAIT_STAND;
        gait_setConfig(cfg);
        req->send(200, "text/plain", "ok");
    });

    // Telemetry: /telem
    server.on("/telem", HTTP_GET, [](AsyncWebServerRequest* req) {
        String json = "{";
#if FEATURE_INA219
        json += "\"v\":" + String(batteryVoltage, 2) + ",";
        json += "\"i\":" + String(batteryCurrent, 2);
#else
        json += "\"v\":0,\"i\":0";
#endif
        json += "}";
        req->send(200, "application/json", json);
    });

    // Serve index.html.gz from PROGMEM
    extern const uint8_t index_html_gz[];
    extern const uint32_t index_html_gz_len;
    server.on("/", HTTP_GET, [&](AsyncWebServerRequest* req) {
        AsyncWebServerResponse* res = req->beginResponse_P(200, "text/html",
            index_html_gz, index_html_gz_len);
        res->addHeader("Content-Encoding", "gzip");
        req->send(res);
    });

    server.begin();
}

// ─────────────────────────────────────────────
//  Setup
// ─────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    Serial.println("\n── ESP32 Robot Dog ──");

    // Servo init
    servoHAL_init();
    servoHAL_centerAll();

    // WiFi AP
    WiFi.softAP(APssid, APpass);
    Serial.printf("AP: %s  IP: %s\n", APssid, WiFi.softAPIP().toString().c_str());

    // Web server
    setupRoutes();

#if FEATURE_INA219
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    ina219.init();
#endif

    // Gait task on Core 0
    xTaskCreatePinnedToCore(gaitTask, "gait", 4096, NULL, 1, &gaitTaskHandle, 0);

    GaitConfig standCfg;
    standCfg.type = GAIT_STAND;
    gait_setConfig(standCfg);

    Serial.println("Ready. Type 'set help' for CLI commands.");
}

// ─────────────────────────────────────────────
//  Loop — Serial CLI + telemetry poll
// ─────────────────────────────────────────────
void loop() {
    // Serial CLI
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd.length()) handleCLI(cmd);
    }

#if FEATURE_INA219
    static unsigned long lastTelem = 0;
    if (millis() - lastTelem > 1000) {
        lastTelem     = millis();
        batteryVoltage = ina219.getBusVoltage_V();
        batteryCurrent = ina219.getCurrent_mA();
    }
#endif
}
