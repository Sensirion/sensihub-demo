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

As this code relies on three fonts that are buried within the TFT_eSPI library which the Arduino IDE generally can't find, we must copy those fonts into this workspace (i.e. the same folder, where this README is in).

For this either manually unzip the ZIP download or find the folder where the library is installed on your system. Then go to `TFT_eSPI/Fonts/Custom/fonts/` and copy the following three files into this workspace:
 - SensirionSimple25pt7b.h
 - ArchivoNarrow_Regular10pt7b.h
 - ArchivoNarrow_Regular50pt7b.h
