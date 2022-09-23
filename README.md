# Sensirion Sensor Hub Demonstrator

Scan for nearby Sensirion Gadgets via BLE and display their data on a Display.

# Platform
This demonstrator was built and tested using an ESP32, but may work with other BLE enabled microprocessors.  

# Setup

# Dependencies

### ArduinoBleSensiScan. 
This library scans for Sesnirion Gadgets in the area and provides their data.

Install ArduinoBle SensiScan

Download the Libaray from https://github.com/MBjoern/arduino-ble-sensiscan as a ZIP file. 

Open the Arduino IDE (note that we assume you use the Arduino IDE 1.8.X or earlier) and install the library by clicking `Sketch -> Include Library -> Add .ZIP Libarary...`. 

### TFT_eSPI
This is a graphics library for 32 bit processors. 

Download the Library from https://github.com/Bodmer/TFT_eSPI as a ZIP file and proceed as above.
Alternatively, you can also install this library using the Arduino Librar Manager.

This sketch relies on three fonts and a configuration file being available to the TFT_eSPI library, which are provided in the `resources` folder and need to be copied over.

For this you need to access the Folder where the Arduino IDE installed the library:
* On Windows, this is by default `C:\Users\Your User Name\Documents\Arduino\libraries`.
* On Ubuntu, it is generally in `~/Arduino/libraries`.
Note that the library may also be called `TFT_eSPI-master`.

Copy the three fonts contained in the `fonts` folder into `TFT_eSPI/Fonts/Custom/`.
Copy the `User_Custom_Fonts.h` to `TFT_eSPI/User/User_Setups/`.