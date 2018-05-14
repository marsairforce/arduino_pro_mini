#ifndef _DHT11_H_
#define _DHT11_H_
// DHT11
// Ispired by the Adafruit DHT library
// (MIT license, written by Adafruit Industries)

#include <inttypes.h>

// when we read from the DHT11 temperature and humitity sensor.
// we only want to actually read from it on the interval we specify.
struct dht11_reading {
  uint32_t read_millis = 0;
  uint32_t period = 60000; // default every minute
  bool success;
  int8_t t_offset = 0;
  int8_t t;
  int8_t h;
};

class DHT11 {
  uint8_t _pin;
  #ifdef __AVR
    // Use direct GPIO access on an 8-bit AVR so keep track of the port and bitmask
    // for the digital pin connected to the DHT.  Other platforms will use digitalRead.
    uint8_t _bit, _port;
  #endif
  uint32_t _maxcycles;
  uint8_t data[5];

  dht11_reading reading;
  uint32_t DHT11::expectPulse(bool level);

  public:
  DHT11(uint8_t pin, int8_t offset = 0);
  void begin();
  dht11_reading read();
};

class InterruptLock {
  public:
   InterruptLock();
   ~InterruptLock();
};

#endif
