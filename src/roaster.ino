// this example is public domain. enjoy!
// www.ladyada.net/learn/sensors/thermocouple

#include <max6675.h>
#include <ModbusRtu.h>

// data array for modbus network sharing
uint16_t au16data[16] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1 };

/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  u8serno : serial port (use 0 for Serial)
 *  u8txenpin : 0 for RS-232 and USB-FTDI 
 *               or any pin number > 1 for RS-485
 */
Modbus slave(1,0,0); // this is slave @1 and RS-232 or USB-FTDI


int thermoCS_bt = 2;
int thermoCLK_bt = 3;
int thermoDO_bt = 4;

int thermoCS_et = 5;
int thermoCLK_et = 6;
int thermoDO_et = 7;

MAX6675 thermocouple_bt(thermoCLK_bt, thermoCS_bt, thermoDO_bt);
MAX6675 thermocouple_et(thermoCLK_et, thermoCS_et, thermoDO_et);

int led = 9;  
  
void setup() {
  slave.begin( 19200); // 19200 baud, 8-bits, even, 1-bit stop
  // use Arduino pins 
  pinMode(led, OUTPUT);
  delay(500);
}

void loop() {
  // basic readout test, just print the current temp

  
   //Serial.print("C = "); 
   au16data[2] = ((uint16_t) thermocouple_bt.readCelsius()*100);
   au16data[3] = ((uint16_t) thermocouple_et.readCelsius()*100);
   slave.poll( au16data, 16 );

   for(int i=1; i<=99; i++) {
    if(i<=au16data[4])
      digitalWrite(led, HIGH);
    else
      digitalWrite(led, LOW);
    
    delay(5);
   }
   
}
