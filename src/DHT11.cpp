#include "arduino_pro_mini.h"
#include <Arduino.h>

// Ispired by the Adafruit DHT library
// (MIT license, written by Adafruit Industries)

// Uncomment to enable printing out nice debug messages.
#define DHT_DEBUG

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef DHT_DEBUG
  #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
  #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define DEBUG_PRINT(...) {}
  #define DEBUG_PRINTLN(...) {}
#endif


DHT11::DHT11(uint8_t pin, int8_t offset) {
    _pin = pin;

    #ifdef __AVR
    _bit = digitalPinToBitMask(pin);
    _port = digitalPinToPort(pin);
    #endif
    _maxcycles = microsecondsToClockCycles(1000);
    // 1 millisecond timeout for reading pulses from DHT sensor.
    // Note that count is now ignored as the DHT reading algorithm adjusts itself
    // based on the speed of the processor.

    reading.t_offset = offset;
}

void DHT11::begin() {
    pinMode(_pin, INPUT_PULLUP);
}

// really the thing we do differently here is return a structure of all the values at once.
dht11_reading DHT11::read() {
    uint32_t current_millis = millis();

    if (current_millis - reading.read_millis > reading.period || reading.read_millis == 0) {
        // Reset 40 bits of received data to zero.
        data[0] = data[1] = data[2] = data[3] = data[4] = 0;

        // Go into high impedence state to let pull-up raise data line level and
        // start the reading process.
        digitalWrite(_pin, HIGH);
        delay(250);

        // First set data line low for 20 milliseconds.
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, LOW);
        delay(20);

        uint32_t cycles[80];
        {
        // Turn off interrupts temporarily because the next sections are timing critical
        // and we don't want any interruptions.
        InterruptLock lock;

        // End the start signal by setting data line high for 40 microseconds.
        digitalWrite(_pin, HIGH);
        delayMicroseconds(40);

        // Now start reading the data line to get the value from the DHT sensor.
        pinMode(_pin, INPUT_PULLUP);
        delayMicroseconds(10);  // Delay a bit to let sensor pull data line low.

        // First expect a low signal for ~80 microseconds followed by a high signal
        // for ~80 microseconds again.
        if (expectPulse(LOW) == 0) {
            reading.success = false;
            goto exit;
        }
        if (expectPulse(HIGH) == 0) {
            DEBUG_PRINTLN(F("Timeout waiting for start signal high pulse."));
            reading.success = false;
            goto exit;
        }

        // Now read the 40 bits sent by the sensor.  Each bit is sent as a 50
        // microsecond low pulse followed by a variable length high pulse.  If the
        // high pulse is ~28 microseconds then it's a 0 and if it's ~70 microseconds
        // then it's a 1.  We measure the cycle count of the initial 50us low pulse
        // and use that to compare to the cycle count of the high pulse to determine
        // if the bit is a 0 (high state cycle count < low state cycle count), or a
        // 1 (high state cycle count > low state cycle count). Note that for speed all
        // the pulses are read into a array and then examined in a later step.
        for (int i=0; i<80; i+=2) {
            cycles[i]   = expectPulse(LOW);
            cycles[i+1] = expectPulse(HIGH);
        }
    } // Timing critical code is now complete.

    // Inspect pulses and determine which ones are 0 (high state cycle count < low
    // state cycle count), or 1 (high state cycle count > low state cycle count).
    for (int i=0; i<40; ++i) {
        uint32_t lowCycles  = cycles[2*i];
        uint32_t highCycles = cycles[2*i+1];
        if ((lowCycles == 0) || (highCycles == 0)) {
            DEBUG_PRINTLN(F("Timeout waiting for pulse."));
            reading.success = false;
            goto exit;
        }
        data[i/8] <<= 1;
        // Now compare the low and high cycle times to see if the bit is a 0 or 1.
        if (highCycles > lowCycles) {
            // High cycles are greater than 50us low cycle count, must be a 1.
            data[i/8] |= 1;
        }
        // Else high cycles are less than (or equal to, a weird case) the 50us low
        // cycle count so this must be a zero.  Nothing needs to be changed in the
        // stored data.
    }

    DEBUG_PRINTLN(F("Received:"));
    DEBUG_PRINT(data[0], HEX); DEBUG_PRINT(F(", "));
    DEBUG_PRINT(data[1], HEX); DEBUG_PRINT(F(", "));
    DEBUG_PRINT(data[2], HEX); DEBUG_PRINT(F(", "));
    DEBUG_PRINT(data[3], HEX); DEBUG_PRINT(F(", "));
    DEBUG_PRINT(data[4], HEX); DEBUG_PRINT(F(" =? "));
    DEBUG_PRINTLN((data[0] + data[1] + data[2] + data[3]) & 0xFF, HEX);

    // Check we read 40 bits and that the checksum matches.
    if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {

        // temp
        reading.t = data[2] + reading.t_offset;

        // humidity
        reading.h = data[0];

        reading.success = true;
    }
    else {
        DEBUG_PRINTLN(F("Checksum failure!"));
        reading.success = false;
    }
exit:
        reading.read_millis = current_millis;
    } // if not read in a while
    return reading;
}

// Expect the signal line to be at the specified level for a period of time and
// return a count of loop cycles spent at that level (this cycle count can be
// used to compare the relative time of two pulses).  If more than a millisecond
// ellapses without the level changing then the call fails with a 0 response.
// This is adapted from Arduino's pulseInLong function (which is only available
// in the very latest IDE versions):
//   https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/wiring_pulse.c
uint32_t DHT11::expectPulse(bool level) {
  uint32_t count = 0;
  // On AVR platforms use direct GPIO port access as it's much faster and better
  // for catching pulses that are 10's of microseconds in length:
  #ifdef __AVR
    uint8_t portState = level ? _bit : 0;
    while ((*portInputRegister(_port) & _bit) == portState) {
      if (count++ >= _maxcycles) {
        return 0; // Exceeded timeout, fail.
      }
    }
  // Otherwise fall back to using digitalRead (this seems to be necessary on ESP8266
  // right now, perhaps bugs in direct port access functions?).
  #else
    while (digitalRead(_pin) == level) {
      if (count++ >= _maxcycles) {
        return 0; // Exceeded timeout, fail.
      }
    }
  #endif

  return count;
}