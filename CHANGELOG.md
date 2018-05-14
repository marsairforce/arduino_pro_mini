# Changes

## (2018-04-25)
* Integrated a library for DS1307 RTC with support for USIWire.
* Split the DHT11 to its own header file.

## (2018-04-17)
* Create a class for DHT11 temperature and humidity sensor. Based off Adafruit library
  * only this one does not support float values.
  * Returns a single structure for temperature, humidity, result status.
  * Includes a built-in offset for temperature, so we can adjust for inaccuracies between sensors and environment.
  * None of the fancy heat index computations, or Farenheit. Since these are inaccurate with single digit integer precision out of the DHT11 anyway.

## (2018-03-29)
* support the on board LED (port 13) as an object.
* support the piezo buzzer as our convention on pin 11

