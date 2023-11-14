#include <util/delay.h>
#include <Arduino.h>
#include <avr/io.h>
#include <DHT11.h>
// extern "C" {
//   #include "dht11.h"
// }

#define SOUND_ANALOG_PIN A0
#define DHT11_DIGITAL_PIN 5
#define CO2_PIN A4

DHT11 dht11(DHT11_DIGITAL_PIN);

void setup() {
  Serial.begin(115200);
  delay(1000);


  pinMode(SOUND_ANALOG_PIN, INPUT);
}

uint16_t counter = 0;
void loop() {
  if(counter == 1500) {
       
    int humidity = dht11.readHumidity();
    if (humidity != DHT11::ERROR_CHECKSUM && humidity != DHT11::ERROR_TIMEOUT){
      Serial.print("H"); Serial.print(humidity); Serial.print("\n");
    }

    int temperature = dht11.readTemperature();
    if(temperature != DHT11::ERROR_CHECKSUM && temperature != DHT11::ERROR_TIMEOUT) {
      Serial.print("T"); Serial.print(temperature); Serial.print("\n");
    }
  }
    
  if (counter == 3000){
    int co2 = analogRead(CO2_PIN);
    Serial.print("C"); Serial.print(co2); Serial.print("\n");

    counter = 0;
  }
  
  
  int valSound = analogRead(SOUND_ANALOG_PIN);
  Serial.print("S"); Serial.print(valSound); Serial.print("\n");
  delayMicroseconds(200);

  counter++;
}
