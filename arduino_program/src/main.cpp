#include <util/delay.h>
#include <Arduino.h>
#include <avr/io.h>
#include <dht.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2,3);

#define SOUND_ANALOG_PIN A0
#define DHT11_DIGITAL_PIN 5
#define CO2_PIN A4

uint8_t counterTimer1 = 0;
uint16_t counterTimer2 = 0;

bool readDHT = false;
bool readSound = false;
bool readCO2 = false;
bool wifiCommandMode = true;

dht DHT;

String commandSendBuffer;

void getTempHumid() {
  int chk = DHT.read11(DHT11_DIGITAL_PIN);

  if (chk == 0){
    mySerial.print("T"); mySerial.print((int) DHT.temperature); mySerial.print("\n");
    mySerial.print("H"); mySerial.print((int) DHT.humidity); mySerial.print("\n");
    // Serial.print("T"); Serial.print((int) DHT.temperature); Serial.print("\n");
  }
}

//---------------------TIMER SETUP------------------------//
void initTimer1() {
  // Set Timer 1 to CTC mode
  TCCR1A = 0;
  TCCR1B = 0;

  // Set the CTC mode
  TCCR1A |= (1 << WGM12);

  OCR1A = 16000;

  TIMSK1 |= (1 << OCIE1A);
}

void startTimer1() { // Start Timer 1
  TCCR1B |= (1 << CS12); // prescaler 256
}

void stopTimer1() { // Stop timer 1
  TCCR1B &= ~(_BV(CS12));
}

void initTimer2() {
  // Set Timer 2 to Fast PWM mode
  TCCR2A = 0;
  TCCR2B = 0;

  // Set to Fast PWM Mode 
  TCCR2A |= (1 << WGM22) | (1 << WGM20);

  OCR2A = 255;

  TIMSK2 |= (1 << OCIE2A);
}

void startTimer2() { // Start timer 2
  TCCR2B |= _BV(CS20); // Prescaler 1 
}

void stopTimer2() { // Stop timer 2
  TCCR2B &= ~(_BV(CS20));
}

ISR(TIMER1_COMPA_vect) { // Timer 1 interupt
  counterTimer1++;

  if (counterTimer1 == 1){
    readDHT = true;
  } else if (counterTimer1 == 2){
    counterTimer1 = 0;
    readCO2 = true;
  }
}


ISR(TIMER2_COMPA_vect) { // Timer 2 interupt 
    counterTimer2++;
    if (counterTimer2 == 80){ // I don't know what speed we want to collect at, either 20 or 40 for the counter value
      counterTimer2 = 0;
      readSound = true;
    }
}
//--------------------------------------------------------//

uint16_t startTimer = 10000;

void setup() {
  Serial.begin(57600);
  mySerial.begin(57600);
  mySerial.listen();
  delay(100);

  pinMode(SOUND_ANALOG_PIN, INPUT);
  pinMode(DHT11_DIGITAL_PIN, INPUT);
  pinMode(CO2_PIN, INPUT);

  initTimer1();
  initTimer2();

  commandSendBuffer = "";
}

void loop() { 
  if (startTimer > 0){
    startTimer--;
    delay(1);
  }

  if (startTimer == 1){
    wifiCommandMode = false;
    startTimer1();
    startTimer2();
  }

  if (!wifiCommandMode){
    if (readDHT){
      getTempHumid();
      readDHT = false;
    }

    if (readSound){
      readSound = false;
      int valSound = analogRead(SOUND_ANALOG_PIN);
      mySerial.print("S"); mySerial.print(valSound); mySerial.print("\n");
      // Serial.print("S"); Serial.print(valSound); Serial.print("\n");
    }

    if (readCO2){
      int co2 = analogRead(CO2_PIN);
      mySerial.print("C"); mySerial.print(co2); mySerial.print("\n");
      // Serial.print("C"); Serial.print(co2); Serial.print("\n");
      readCO2 = false;
    }
  }

  while(mySerial.available()) {
    Serial.print((char) mySerial.read());
  }

  while(Serial.available()) {
    char c = (char) Serial.read();
    if(c == '\n') {
      delay(100);
      if (commandSendBuffer == "SENDING OFF"){
          wifiCommandMode = true;
          stopTimer1();
          stopTimer2();
          Serial.println("Ready for wifi commands...");
      } else if (commandSendBuffer == "SENDING ON"){
          wifiCommandMode = false;
          Serial.println("Data sending begins");
          startTimer1();
          startTimer2();
      } else {
        mySerial.print(commandSendBuffer); mySerial.print("\n");
      }
      commandSendBuffer = "";
      delay(100);
    } else if (c == '\r'){
      Serial.println("r Shown");
    } else {
      commandSendBuffer += c;
    }
  }
}
