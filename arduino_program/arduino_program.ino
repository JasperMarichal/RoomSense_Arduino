#include <SoftwareSerial.h>
SoftwareSerial mySerial(2,3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  mySerial.begin(57600);
  mySerial.listen();

}

void loop() {
  // put your main code here, to run repeatedly:
  String incomingString="";
  boolean stringReady = false;

  while(mySerial.available()) {
    Serial.print((char) mySerial.read());
    //incomingString += (char) mySerial.read();
    //stringReady = true;
  }

  while(Serial.available()) {
    mySerial.print((char) Serial.read());
    //incomingString += (char) mySerial.read();
    //stringReady = true;
  }
  //if (stringReady) {
    //Serial.println("Recieved String: " + incomingString);
  //}
}