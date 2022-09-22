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