#include <Arduino.h>

#define SOUND_ANALOG_PIN A0

void setup() {
  Serial.begin(115200);
  pinMode(SOUND_ANALOG_PIN, INPUT);
}

void loop() {
  int val = analogRead(SOUND_ANALOG_PIN);
  Serial.print(val); Serial.print("\n");
  delayMicroseconds(20);
}

