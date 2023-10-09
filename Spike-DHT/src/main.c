#define __DELAY_BACKWARD_COMPATIBLE__  
#include <util/delay.h>
#include <usart.h>
#include <avr/io.h>

#define DATA_PIN PD6
uint8_t bits[5];
uint8_t cnt;
uint8_t idx;

int dataReading(){
  
  cnt = 7;
  idx = 0;
  for (int i=0; i< 5; i++) bits[i] = 0;

  _delay_ms(1);
  DDRD |= (1 << DATA_PIN); // Set as output
  PORTD |= (1 << DATA_PIN); // Set high
  _delay_ms(1);

  // MCU sends start signal
  // Set data pin low for at least 18ms
  DDRD |= (1 << DATA_PIN); // Set as output
  PORTD &= ~(1 << DATA_PIN); // Set low
  _delay_ms(18);

  // Pull up and wait 20-40us for DHT's response
  PORTD |= (1 << DATA_PIN); // Set high
  _delay_us(40);

  // Change data pin to input for receiving response
  DDRD &= ~(1 << DATA_PIN);

  // DHT prepares for data transmission (80us)

  while ((PIND & (1 << DATA_PIN)) == 0) _delay_us(1); // Wait for rising edge

  while ((PIND & (1 << DATA_PIN)) != 0) _delay_us(1); // Wait for falling edge

  // Read 40 bits of data and store in an array
  // uint8_t capturedData[ARRAY_SIZE];

  for (int i = 0; i < 40; i++) {

        while ((PIND & (1 << DATA_PIN)) == 0) _delay_us(1); // Wait for rising edge

        uint8_t times = 0;

        while((PIND & (1 << DATA_PIN)) != 0){
          _delay_us(5);
          times += 5;
        }

        // Wait for 26-28us or 70us to determine bit value
        if (times > 40) bits[idx] |= (1 << cnt);

        if (cnt == 0)   // next byte?
        {
          cnt = 7;    // restart at MSB
          idx++;      // next byte!
        }
        else cnt--;

  }

  _delay_ms(1);
  DDRD |= (1 << DATA_PIN); // Set as output
  PORTD |= (1 << DATA_PIN); // Set high
  _delay_ms(1);

  int humidity    = bits[0]; 
	int temperature = bits[2]; 

	uint8_t sum = bits[0] + bits[1] + bits[2] + bits[3];

  if (bits[4] != sum) printString("BAD DATA\n");

  // printBinaryByte(humidity);
  // printString("\n");
  // printBinaryByte(temperature);
  // printString("\n");
  // printBinaryByte(bits[4]);

  //   printString("\n\n");
  // printBinaryByte(bits[0]);
  //   printString("\n");
  // printBinaryByte(bits[1]);
  // printString("\n");
  // printBinaryByte(bits[2]);
  //   printString("\n");
  // printBinaryByte(bits[3]);
  // printString("\n");
  // printBinaryByte(bits[4]);
  // printString("\nReal Numbers:\n");

  printf("Humidity: %d %%\n", humidity);
  printf("Temperature: %d C\n\n", temperature);

  // _delay_ms(2000);
  return 0;
}

int main() {
  initUSART();
  printString("Running\n");
  _delay_ms(3000);

  dataReading();
  _delay_ms(3000);

  while (1){
  // for (uint8_t i = 0; i < 50; i++){
    dataReading();
    _delay_ms(3000);
  }
  
  

  return 0;
}


