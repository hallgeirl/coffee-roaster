/*
This is a slightly modified version of the adafruit library at: https://github.com/adafruit/MAX6675-library
The change is mostly to replace the calls to delay(1) with a shorter delay (otherwise each reading takes about 16ms!)
*/

// this library is public domain. enjoy!
// www.ladyada.net/learn/sensors/thermocouple

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class MAX6675 {
 public:
  MAX6675(int8_t SCLK, int8_t CS, int8_t MISO);

  double readCelsius(void);
  double readFahrenheit(void);
  // For compatibility with older versions:
  double readFarenheit(void) { return readFahrenheit(); }
 private:
  int8_t sclk, miso, cs;
  uint8_t spiread(void);
};
