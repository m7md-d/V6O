# V6O Project

The V6O project is based on making a V60-style drip machine. 

## Components
* 12V water pump.
* Servo.
* ULN2003.
* Arduino Nano ESP32.
* Relay.
* Button.
* kettle.
* Dripper.
* Rotating plate that spins the dripper.
* Two small hoses.

## Requirements
* [Arduino IDE](https://www.arduino.cc/en/software) with the [ESP32 board package](https://github.com/espressif/arduino-esp32).
* [Stepper](https://github.com/arduino-libraries/Stepper) library.
* [ESP32Servo](https://github.com/madhephaestus/ESP32Servo) library.

## Process
### Starts in a specific way.
1. **First Way:**
- Press the button on the hardware side and it will start automaticly in default process.

2. **Second Way:**
- using the UI that is included to chose the pours and the time between pours and the amount of time waiting between pours.

## Showcase Video

<p align="center">
  <img src="assets/showcase.gif" alt="V6O Demo" />
</p>
