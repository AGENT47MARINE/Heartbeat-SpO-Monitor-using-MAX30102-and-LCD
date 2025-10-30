# 💓 Heartbeat & SpO₂ Monitor using MAX30102

This Arduino project reads pulse data from the MAX30102 sensor and displays BPM with a simple animated SpO₂ bar on an I2C LCD.

## ⚙️ Features
- Real-time heart rate detection
- Animated SpO₂-style pulse bar
- Finger detection with “Place Finger” prompt
- Clean, beginner-friendly code

## 🧰 Hardware
- Arduino Uno / Nano / ESP32
- MAX30102 Pulse Oximeter Sensor
- 16x2 I2C LCD Display
- Jumper Wires

## 🔌 Connections
| Component | Pin | Voltage |
|------------|-----|----------|
| MAX30102 SDA | A4 | 3.3V |
| MAX30102 SCL | A5 | 3.3V |
| LCD SDA | A4 | 3.3V |
| LCD SCL | A5 | 3.3V |
| VCC (both) | 3.3V |
| GND | GND |

## 🧩 Libraries
- Wire.h  
- MAX30105.h  
- heartRate.h  
- LiquidCrystal_I2C.h

## 🪄 How It Works
1. The sensor detects IR reflections from your finger.
2. BPM is calculated from the intervals between heartbeats.
3. The LCD displays live BPM and an animated SpO₂ bar.

## 📸 Demo
![LCD Output](images/output_display.jpg)

## 📝 License
MIT License
