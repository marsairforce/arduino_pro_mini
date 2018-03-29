
#include "arduino_pro_mini.h"
#include <Arduino.h>

// Constructors
ArduinoLED::ArduinoLED() {
  this->setup(ARDUINO_PRO_MINI_PIN_LED);
}

ArduinoLED::ArduinoLED(int pin) {
  this->setup(pin);
}

void ArduinoLED::setup(int pin) {
  this->m_pin = pin;
  pinMode(this->m_pin, OUTPUT);
}


void ArduinoLED::on() {
  digitalWrite(m_pin, HIGH);
}

void ArduinoLED::off() {
  digitalWrite(m_pin, LOW);
}

void ArduinoLED::blink(int on_time, int off_time, int repeat) {
  for (int i = 0; i < repeat; i++) {
    this->on();
    delay(on_time);
    this->off();
    delay(off_time);
  }
}
