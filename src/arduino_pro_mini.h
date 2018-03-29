#ifndef _arduino_pro_mini_h_
#define _artuino_pro_mini_h_

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

#endif
