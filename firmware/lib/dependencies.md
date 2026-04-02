# Library Dependencies

Install via **Arduino IDE → Sketch → Include Library → Manage Libraries**  
unless noted as *manual install*.

| Library | Version | Source |
|---------|---------|--------|
| ESP Async Web Server | latest | [GitHub](https://github.com/me-no-dev/ESPAsyncWebServer) — manual |
| AsyncTCP | latest | [GitHub](https://github.com/me-no-dev/AsyncTCP) — manual |
| ESP32 ISR Servo | **1.1.0** | Library Manager |
| MPU9250_WE | **1.1.3** | Library Manager |
| INA219_WE | latest | Library Manager |

## Board Package

Use ESP32 Arduino Core **v1.x only**.  
v2.x introduced WiFi task changes that cause one core to freeze.

Add to Arduino → Preferences → Additional Board URLs:
```
https://dl.espressif.com/dl/package_esp32_index.json
```

Board: **ESP32 Dev Module**  
Upload Speed: 921600  
Flash Size: 4MB  
Partition Scheme: Default
