# Sensirion Sensor Hub Demonstrator

Scan for nearby Sensirion Gadgets via BLE and display their data on a Display.

# Platform
This demonstrator was built and tested using an ESP32 with a TTGO display, which uses an adapted version of the TFT_eSPI library.

# Dependencies

## ArduinoBleSensiScan. 
This library scans for Sesnirion Gadgets in the area and provides their data.

Install ArduinoBle SensiScan

Download the Libaray from https://github.com/MBjoern/arduino-ble-sensiscan as a ZIP file. 

Open the Arduino IDE (note that we assume you use the Arduino IDE 1.8.X or earlier) and install the library by clicking `Sketch -> Include Library -> Add .ZIP Libarary...`. 

## NimBLE
This is the Bluetooth Low Energy (BLE) library the ArduinoBleSensiScan library is built upon.

Download the Library from https://github.com/h2zero/NimBLE-Arduino as a ZIP file and proceed as above.

## TFT_eSPI: TTGO
This is a graphics library for 32 bit processors. 

Download the Library from https://github.com/Xinyuan-LilyGO/TTGO-T-Display as a ZIP file and proceed as above.

This sketch relies on three fonts and a configuration file being available to the TFT_eSPI library, which are provided in the `resources` folder and need to be copied over.

For this you need to access the Folder where the Arduino IDE installed the library:
* On Windows, this is by default `C:\Users\Your User Name\Documents\Arduino\libraries`.
* On Ubuntu, it is generally in `~/Arduino/libraries`.

Copy the three fonts contained in the `fonts` folder into `TFT_eSPI/Fonts/Custom/`.
Copy the `User_Custom_Fonts.h` to `TFT_eSPI/User_Setups/`.