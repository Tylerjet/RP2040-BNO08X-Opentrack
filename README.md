# RP2040-BNO08X-Opentrack
This repository contains the platformio project and model file(s) for creating a headtracker for use with opentrack. The RP2040 takes the data from the BNO08X via SPI connection and translates that data into joystick inputs for opentrack.

I used the RP2040 Zero But in theory any RP2040 should work as long as the board has enough pins for SPI communication.

You will need to enable the P0 and P1 jumpers/contacts to enable SPI mode on the BNO08X. This will vary per board so check your documentation for how to enable SPI mode.

# Configuration

# Wiring
>Note: The below use of MOSI/MISO terminology is considered obsolete. SDO/SDI is now used. The MOSI signal on a controller can be replaced with the title ‘SDO’. Master and Slave are now Controller and Peripheral. Additional information can be found [here](https://www.oshwa.org/a-resolution-to-redefine-spi-signal-names/).

RP2040 | BNO08X
--- | ---
3v3 | VCC
GND | GND
SPI0 CSn | CS
SPI0 SCK | SCL/SCK/RX
SPI0 TX | ~MOSI~/SDO/Addr
SPI0 RX | ~MISO~/SDI/TX
Any Unused GPIO | INT
Any Unused GPIO | RST
|| PS1 Bridged
|| PS0 Bridged

RP2040 Zero Example:

![image](https://github.com/user-attachments/assets/fed037af-660b-4953-8992-dca25a5248ae)

BNO08X Example:

![image](https://github.com/user-attachments/assets/796a5d6a-8771-434a-b6f9-4e14a5c8210c)


# TODO
Add Full instructions (and images?) for wiring and building hardware
