# Things when working with Arduino Pro Mini

That shitty LED that is built-into the device. Kind of removes the ability to use pin 13.

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

Had not place to put this, so stuffed it into this project. Which makes sense since most of the shitty gadgets I use are Arduino Pro Mini based so include this already anyway. Libraries kind of suck in Arduino IDE. So let's not make a ton of them.
