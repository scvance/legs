# Description
This repository contains all of the code to create the bodyless legs that I have built.
This assumes an arduino Mega-2560 for the leg_controller.ino and an arduino nano 5v for the slave_leg.ino

You could also just buy some arduino board that has 6 external interrupt pins AND doesn't use any of them when I2C is enabled. That would remove the need for two microcontrollers (just saying)

The circuit board gerber file and the 3D print files will be found on my website once I make it lol
I'll also stick a wiring diagram up there too.

RN the code just reads from all motors and sensors. No walking code is uploaded yet.

## parts list
- Arduino Mega Pro
- Arduino Nano
- 7 MPU-6050
- TCA9548A multiplexer
- 4 large NXT/EV3 servo motors
- 2 medium EV3 servo motors
- 3 sn754410 motor drivers
- capacitors <-- include the rest later.

## Dependencies
- Wire.h
- MPU6050.h
- PWFusion_TCA9548A.h
