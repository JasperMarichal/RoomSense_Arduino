#include <SoftwareSerial.h>
SoftwareSerial espSerial(2,3); 
/* 
  The Serial is the connection to a computer if there is one connected to the arduino via the USB port.
  The espSerial is the serial connection to the ESP wifi module
  The arduino acts as a "man in the middle" and transmits all data coming from one direction to the other one and vice versa
  The arduino will (later) listen to (a very limited set of) commands, but only from the esp, so that we can make the RGB led change color, or to activate
  a relay when the system deems it necessary (for ventilation).
 */

void setup() {
  Serial.begin(57600);
  espSerial.begin(57600);
  espSerial.listen();
}

void loop() {

  while(espSerial.available()) {
    Serial.print((char) espSerial.read());
  }

  while(Serial.available()) {
    espSerial.print((char) Serial.read());
  }
}