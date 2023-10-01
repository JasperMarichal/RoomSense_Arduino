#include <Arduino.h>

#define SOUND_ANALOG_PIN A0

void setup() {
  Serial.begin(9600);
  Serial.println("Hello world!");
  pinMode(SOUND_ANALOG_PIN, INPUT);
}

void loop() {
  Serial.print(analogRead(SOUND_ANALOG_PIN)); Serial.print("/1023\n");
  delay(10);
}
