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


int thermoCS_bt = 11;
int thermoCLK_bt = 10;
int thermoDO_bt = 12;
int thermoVCC_bt = 9;
int thermoGND_bt = 8;

int thermoCS_et = 4;
int thermoCLK_et = 3;
int thermoDO_et = 5;
int thermoVCC_et = 2;

int mosfetGate = 7;

MAX6675 thermocouple_bt(thermoCLK_bt, thermoCS_bt, thermoDO_bt);
MAX6675 thermocouple_et(thermoCLK_et, thermoCS_et, thermoDO_et);

int led = LED_BUILTIN;
unsigned long intervalMs = 500;
unsigned long startMillis;

unsigned long modbusPollIntervalMs = 1000;
unsigned long modbusPollStartMillis;


void setup() {
  slave.begin(19200); // 19200 baud, 8-bits, even, 1-bit stop
  // use Arduino pins 
  pinMode(led, OUTPUT);
  pinMode(thermoVCC_bt, OUTPUT);
  pinMode(thermoGND_bt, OUTPUT);
  pinMode(thermoVCC_et, OUTPUT);
  pinMode(mosfetGate, OUTPUT);
  digitalWrite(thermoVCC_bt, HIGH);
  digitalWrite(thermoGND_bt, LOW);
  digitalWrite(thermoVCC_et, HIGH);
  digitalWrite(mosfetGate, LOW);

  startMillis = millis();
  modbusPollStartMillis = millis();
}
bool debug = false;
char buf[256];
int heatAmount = 0;

void loop()
{
  look_sync();
}

void look_sync()
{
  au16data[2] = ((uint16_t) (thermocouple_bt.readCelsius()*100.0));
  au16data[3] = ((uint16_t) (thermocouple_et.readCelsius()*100.0));
  slave.poll( au16data, 16 );
  int heatAmountInt = au16data[4]; // 0-100 value into "how many ms should I stay on"  
  
  for (int i = 0; i < 100; i++)
  {
    if (i < heatAmountInt) 
    {
      digitalWrite(mosfetGate, HIGH);
      digitalWrite(led, HIGH);      
    }
    else
    {
      digitalWrite(mosfetGate, LOW);
      digitalWrite(led, LOW);
    }
    delay(5);
  }
}

void loop_async() {
  unsigned long currentMillis = millis();
  if (currentMillis - modbusPollStartMillis >= modbusPollIntervalMs) {
    au16data[2] = ((uint16_t) (thermocouple_bt.readCelsius()*100.0));
    au16data[3] = ((uint16_t) (thermocouple_et.readCelsius()*100.0));
    slave.poll( au16data, 16 );
    heatAmount = (((float)au16data[4])/100.f)*(float)intervalMs; // 0-100 value into "how many ms should I stay on"  
    if (debug) {
      Serial.print("Polled.");
      ltoa(au16data[2], buf, 10);
      Serial.print(" bt: ");
      Serial.print(buf);

      ltoa(au16data[3], buf, 10);
      Serial.print(" et: ");
      Serial.println(buf);
    }
   
    modbusPollStartMillis = millis();
  }
  
  if ((currentMillis-startMillis) < heatAmount) {
    digitalWrite(mosfetGate, HIGH);
    digitalWrite(led, HIGH);
  }
  else {
    digitalWrite(mosfetGate, LOW);
    digitalWrite(led, LOW);
  }

  if (debug) {
    Serial.print("heatAmount: ");
    dtostrf(heatAmount, 4, 2, buf);
    Serial.print(buf);
    
    Serial.print(" currentMillis: ");
    ltoa(currentMillis, buf, 10);
    Serial.print(buf);
    
    Serial.print(" startMillis: ");
    ltoa(startMillis, buf, 10);
    Serial.println(buf);
  }
  

  if (currentMillis - startMillis >= intervalMs) {
    unsigned long err = (currentMillis - startMillis)-intervalMs;
    startMillis = millis();

    if (debug) {
      Serial.print("err: ");
      ltoa(err, buf, 10);
      Serial.println(buf);
    }
  }
}
