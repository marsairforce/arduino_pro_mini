#ifndef _arduino_pro_mini_h_
#define _artuino_pro_mini_h_

#include <inttypes.h>
#include <Arduino.h>

// The shitty LED built into the Arduino pro mini
#define ARDUINO_PRO_MINI_PIN_LED         13


struct ArduinoLED {
  private:
    int m_pin;

  public:
    ArduinoLED(); // default constuctor, will use the usual pin
    ArduinoLED(int pin); // Allow us to use a different pin

    void setup(int pin);
    void on();
    void off();

    /**
    * Blinks the LED, on and then off
    * With delay `period` between each cycle.
    * Optional repeat parameter, default to 1
    */
    void blink(int on_time, int off_time, int repeat);
    void blink(int period, int repeat = 1) { blink(period, period, repeat); }

};


// Buzzer
// This is just a convention I use.
// Any PWM pin will work (3,5,6,9,10,11)
#define ARDUINO_PRO_MINI_PIN_BUZZER  11

struct PiezoBuzzer {
  private:
    int m_pin;

  public:
  PiezoBuzzer();
  PiezoBuzzer(int pin);

  void setup(int pin);

  void clickSound();
  void errorSound();
  void acknowledgeSound();
  void warningSound();
};



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

// DHT11
// Ispired by the Adafruit DHT library
// (MIT license, written by Adafruit Industries)

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
   InterruptLock() {
    noInterrupts();
   }
   ~InterruptLock() {
    interrupts();
   }

};


#endif
