# Things when working with Arduino Pro Mini

That shitty LED that is built-into the Arduino Pro Mini. Kind of removes the ability to use pin 13.

I decided I would embrace being stuck with this and use it in my projects, even if only for debugging things.

# Solving problems that were not really a problem.

Clases:
* `ArduinoLED` These are functions for manipulating the LED.
* `Buzzer` Every project should have a piezo buzzer really. These are the boiler plate I have used for this.

# Challenges
The hard part starting this library was considering to use underscores, or CamelCase notation for the structure and function names.

# DHT11 Temperature Sensor
I have a bunch of these. From China. They hook up with a 1 wire interface.

I didn't like any of the libraries I found so made my own.

I didn't really have a place to put this, so I just stuffed it into this project. Which makes sense since most of the shitty gadgets I use are Arduino Pro Mini based so include this already anyway. Libraries kind of suck in Arduino IDE. So let's not make a ton of them.

Serial data debug confirmed the DHT11 only supports 8 bit integer values for temperature and humidity:
```
Received:
24, 0, 14, 0, 38 =? 38
Received:
24, 0, 14, 0, 38 =? 38
Received:
24, 0, 14, 0, 38 =? 38
Received:
24, 0, 14, 0, 38 =? 38
Received:
24, 0, 14, 0, 38 =? 38
```

# DS1307 RTC

I made some small breadboard friendly modules of these.
It is the same IC as used on the popular Adafruit RTC breakout board.  Only I didn't put a spot for a battery on this module. I had some small surface mount IC with a not surface mount oscillator. It is a weird combination of crap to have, but it was cheap from AliExpress so here we are.

But rather than using the usual RTCLib, I opted to create support for this device here, to save space. I also use this with ATTiny devices using USIWire library, which the community versions of this RTClib do not support yet.

See also:
* https://github.com/adafruit/RTClib
* https://github.com/jcw/rtclib
* https://github.com/sparkfun/SparkFun_DS1307_RTC_Arduino_Library/

I think my implementation is most closely based off of the Adafruit/jcw version, since I did not like the Sparkfun version had a static instance of the RTC class. And that it has a member property for the RTC value, and not a DateTime class, like I am used to having to work with.