# RP2040-BNO08X-Opentrack
This repository contains the platformio project and model file(s) for creating a headtracker for use with opentrack. The RP2040 takes the data from the BNO08X via SPI connection and translates that data into joystick inputs for opentrack.

I used the RP2040 Zero But in theory any RP2040 should work as long as the board has enough pins for SPI communication.

You will need to enable the P0 and P1 jumpers/contacts to enable SPI mode on the BNO08X. This will vary per board so check your documentation for how to enable SPI mode.

# Configuration

# TODO
Add instructions (and images?) for wiring and building hardware
