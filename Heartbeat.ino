#include <Wire.h>
#include <MAX30105.h>
#include <LiquidCrystal_I2C.h>
#include "heartRate.h"

MAX30105 particleSensor;
LiquidCrystal_I2C lcd(0x27,16,2);

// Heart rate variables
long lastBeat = 0;
float beatsPerMinute = 0;
int beatAvg = 0;

const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;

// Smoothing IR
const int IR_SMOOTH_SIZE = 10;
long irBuffer[IR_SMOOTH_SIZE];
int irIndex = 0;

// LCD update timing
unsigned long lastLCD = 0;
const unsigned long lcdInterval = 500;

// Finger detected flag
bool fingerDetected = false;

// SpO2 bar variables
int spo2Bar = 0;
bool barDirection = true; // true = forward, false = backward

void setup() {
  Serial.begin(9600);
  Wire.begin();

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Initializing...");

  if (!particleSensor.begin()) {
    Serial.println("MAX30102 not found!");
    lcd.clear();
    lcd.print("Sensor Error!");
    while(1);
  }

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x1F);
  particleSensor.setPulseAmplitudeIR(0x1F);

  Serial.println("Sensor ready! Place finger...");
  lcd.clear();
  lcd.print("Place Finger...");
}

void loop() {
  long irValue = particleSensor.getIR();

  // Detect finger
  if (irValue > 5000 && !fingerDetected) { // adjust threshold if needed
    lcd.clear();
    fingerDetected = true;
  }

  // Smooth IR
  irBuffer[irIndex++] = irValue;
  if (irIndex >= IR_SMOOTH_SIZE) irIndex = 0;

  long irSum = 0;
  for (int i = 0; i < IR_SMOOTH_SIZE; i++) irSum += irBuffer[i];
  long irSmooth = irSum / IR_SMOOTH_SIZE;

  // Beat detection
  if (checkForBeat(irSmooth)) {
    long delta = millis() - lastBeat;
    lastBeat = millis();
    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = (byte)beatsPerMinute;
      rateSpot %= RATE_SIZE;

      beatAvg = 0;
      for (byte i = 0; i < RATE_SIZE; i++) beatAvg += rates[i];
      beatAvg /= RATE_SIZE;
    }
  }

  // Update LCD & Serial
  if (millis() - lastLCD >= lcdInterval && fingerDetected) {
    Serial.print("BPM=");
    if (beatAvg > 0) Serial.println(beatAvg);
    else Serial.println("--");

    // Clear line first to prevent ghost digits
    lcd.setCursor(0,0);
    lcd.print("BPM:    "); // enough spaces to clear old digits
    lcd.setCursor(5,0);
    if (beatAvg > 0) lcd.print(beatAvg);
    else lcd.print("--");

    // SpO2 animation bar
    lcd.setCursor(0,1);
    for (int i = 0; i < 16; i++) {
      if (i == spo2Bar) lcd.print("|");
      else lcd.print(" ");
    }

    // Move bar
    if (barDirection) spo2Bar++;
    else spo2Bar--;
    if (spo2Bar >= 15) barDirection = false;
    if (spo2Bar <= 0) barDirection = true;

    lastLCD = millis();
  }
}
