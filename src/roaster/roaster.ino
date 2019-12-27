// this example is public domain. enjoy!
// www.ladyada.net/learn/sensors/thermocouple

#include "max6675.h"
#include "ModbusRtu.h"

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

unsigned long thermocouplePollIntervalMs = 1000;
unsigned long thermocouplePollStartMillis;


void setup() {
  slave.begin(19200); // 19200 baud, 8-bits, even, 1-bit stop
  // use Arduino pins 
  pinMode(led, OUTPUT);
  pinMode(thermoVCC_bt, OUTPUT);
  pinMode(thermoVCC_et, OUTPUT);
  pinMode(mosfetGate, OUTPUT);
  digitalWrite(thermoVCC_bt, HIGH);
  digitalWrite(thermoVCC_et, HIGH);
  digitalWrite(mosfetGate, LOW);

  startMillis = millis();
  thermocouplePollStartMillis = millis();
}
bool debug = false;
char buf[256];
int heatAmount = 0;

void loop()
{
  loop_async();
}

void debug_printValueLn(const char* label, float value)
{
  if (debug)
  {
    Serial.print(label);
    dtostrf(value, 4, 2, buf);
    Serial.println(buf);
  }
}

void debug_printValue(const char* label, float value)
{
  if (debug)
  {
    Serial.print(label);
    dtostrf(value, 4, 2, buf);
    Serial.print(buf);
    Serial.print(" ");  
  }
}


float filterExp(float measurement, float prevFilteredValue, float weight)
{
  return weight * measurement + (1.0f-weight)*prevFilteredValue;
}

float prevFilteredValueEt = -1;
float prevFilteredValueBt = -1;
float filterWeight = 0.15;
float getFilteredTemperature(MAX6675 thermocouple, float* prevValue)
{
  float reading = thermocouple.readCelsius()*100.0;
  
  if (*prevValue < 0)
    (*prevValue) = reading;

  float filteredValue = filterExp(reading, *prevValue, filterWeight);
  *prevValue = filteredValue;
  
  return filteredValue;
}

void loop_sync()
{
  au16data[2] = ((uint16_t) (getFilteredTemperature(thermocouple_bt, &prevFilteredValueBt)));
  au16data[3] = ((uint16_t) (getFilteredTemperature(thermocouple_et, &prevFilteredValueEt)));
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

bool on = false;
void loop_async() {
  
  unsigned long currentMillis = millis();
  if (currentMillis - thermocouplePollStartMillis >= thermocouplePollIntervalMs) {
    au16data[2] = ((uint16_t) (getFilteredTemperature(thermocouple_bt, &prevFilteredValueBt)));
    au16data[3] = ((uint16_t) (getFilteredTemperature(thermocouple_et, &prevFilteredValueEt)));

    debug_printValue("Polled. BT: ", au16data[2]);
    debug_printValue("ET: ", au16data[3]);
   
    thermocouplePollStartMillis = millis();
  }
  slave.poll( au16data, 16 );
  heatAmount = (((float)au16data[4])/100.f)*(float)intervalMs; // 0-100 value into "how many ms should I stay on"  

  if ((currentMillis-startMillis) < heatAmount) {
    digitalWrite(mosfetGate, HIGH);
    digitalWrite(led, HIGH);
    if (!on)
      Serial.println("Switched ON");
    on = true;
  }
  else {
    digitalWrite(mosfetGate, LOW);
    digitalWrite(led, LOW);
    if (on)
      Serial.println("Switched OFF");
    on = false;
  }

  debug_printValue("heatAmount: ", heatAmount);
  debug_printValue("currentMillis: ", (float)currentMillis);
  debug_printValueLn("startMillis: ", (float)startMillis);
    
  if (currentMillis - startMillis >= intervalMs) {
    unsigned long err = (currentMillis - startMillis)-intervalMs;
    startMillis = millis();

    debug_printValueLn("Error (overshoot in ms): ", (float)err);
  }
}
