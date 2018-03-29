#include "arduino_pro_mini.h"
#include <Arduino.h>


PiezoBuzzer::PiezoBuzzer() {
  this->setup(ARDUINO_PRO_MINI_PIN_BUZZER);
}

PiezoBuzzer::PiezoBuzzer(int pin) {
  this->setup(pin);
}

void PiezoBuzzer::setup(int pin) {
  this->m_pin = pin;
  pinMode(m_pin, OUTPUT);
}

void PiezoBuzzer::clickSound() {
  tone(m_pin, 1568, 18); // G6
}

void PiezoBuzzer::errorSound() {
  tone(m_pin, 103, 600); // G2
}

void PiezoBuzzer::acknowledgeSound() {
  tone(m_pin, 524, 200); // C5
  delay(100);
  tone(m_pin, 1568, 200); // G6
}

void PiezoBuzzer::warningSound() {
  tone(m_pin, 880, 300); // A4
  delay(400);
  tone(m_pin, 880, 300); // A4
}
