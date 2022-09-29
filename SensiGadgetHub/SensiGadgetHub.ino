#include <Arduino.h>
#include <ArduinoBleSensiScan.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "Button2.h"

SensiScan sensiScan = SensiScan();
static int updateIntervalMs = 5000;
static int64_t lastUpdateTimeMs = 0;

// Buttons to control displayed values
#define BUTTON_1            35
#define BUTTON_2            0

Button2 btn_right(BUTTON_1);
Button2 btn_left(BUTTON_2);

// Display related
TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke library, pins defined in User_Setup.h

#define SENSIRION_GREEN 0x6E66
#define sw_name "Sensirion Gadget Hub"
#define sw_version "v0.1"

#define GFXFF 1
#define FF99  &SensirionSimple25pt7b
#define FF90  &ArchivoNarrow_Regular10pt7b
#define FF95  &ArchivoNarrow_Regular50pt7b


UnitType current_unit = UnitType::CO2;
Gadget dummy_gadget = {"dummy", "dummy"};
Gadget& current_gadget = dummy_gadget;

// the knownGadgets object contains all gadgets and their samples
// every update, all available gadget have their samples updated
// and new gadgets are added. 
// specific Gadgets and Samples are identified by their deviceId and
// unitType respectively

std::map<Gadget, std::vector<Sample>> knownGadgets;

// for each unit type one may define what color the value
// sould be displayed in, depending on the value itself
// 0 => green
// 1 => orange
// 2 => red

int getLevel(UnitType unit, float value) {
  switch(unit) {
    case UnitType::UNDEFINED: {
      return 2;
    }
    case UnitType::T: {
      if (value > 18.0 && value < 26.0) {
        return 0;
      } else {
        return 1;
      }
    }
    case UnitType::RH: {
      if (value > 40.0 && value < 60) {
        return 0;
      } else {
        return 1;
      }
    }
    case UnitType::VOC: {
      if (value < 200.0) {
        return 0;
      } else if (value < 400.0) {
        return 1;
      } else {
        return 2;
      }
    }
    case UnitType::CO2: {
      if (value > 9999.0) {
        value = 9999.0;
      }
      if (value < 1000.0) {
        return 0;
      } else if (value < 1600.0) {
        return 1;
      } else {
        return 2;
      }
    }
    default:
      return 0;
  }
  }
  }
}

void setupDisplay() {
  tft.init();
  tft.setRotation(1);
}

void displaySplashScreen() {
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(SENSIRION_GREEN, TFT_WHITE);

  uint8_t defaultDatum = tft.getTextDatum();
  tft.setTextDatum(1); // Top centre

  // draw Sensirion Logo
  tft.setTextSize(1);
  tft.setFreeFont(FF99);
  tft.drawString("B", 120, 40);

  // draw app name and version
  tft.setTextSize(1);
  tft.setFreeFont(FF90);
  tft.setTextDatum(6); // bottom left
  tft.drawString(sw_name, 10, 125);
  tft.setTextDatum(8);  // bottom right
  tft.drawString(sw_version, 230, 125);

  // Revert datum setting
  tft.setTextDatum(defaultDatum);
}

void buttonLoop() {
 btn_left.loop();
 btn_right.loop();  
}


void drawSensorValue(const char* label, String value, const char* unit, const char* device_id, uint16_t level) {
  /*
   * Draw a sensor value onto the screen.
   * We support 3 levels for now: 
   *  - 0 good  environment quality
   *  - 1 average environemnt quality
   *  - 2 poor environemnt quality
   */
  uint8_t defaultDatum = tft.getTextDatum();
  
  // clear screen
  tft.fillScreen(TFT_BLACK);

  // set default font
  tft.setTextSize(1);
  tft.setFreeFont(FF90);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // label on bottom left
  tft.setTextDatum(6);
  tft.drawString(label, 10, 125);

  // device id on bottom right
  tft.setTextDatum(8);
  tft.drawString(device_id, 230, 125);

  // draw sensor value in color detemined by level
  if (level == 2) {
    tft.setTextColor(TFT_RED, TFT_BLACK);
  } else if (level == 1) {
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  } else { // default to good
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
  }
  tft.setTextDatum(8); // bottom right
  tft.setTextSize(1);
  tft.setFreeFont(FF95);
  tft.drawString(value, 195, 105);

  // Draw CO2 unit
  tft.setTextSize(1);
  tft.setFreeFont(FF90);
  tft.drawString(unit, 230, 90);

  // Revert datum setting
  tft.setTextDatum(defaultDatum);  
}

void display(float value, UnitType unit, const char* device_id) {
  drawSensorValue(unitTypeString[unit].c_str(), String(value),
    unitTypeSymbols[unit].c_str(), device_id, getLevel(unit, value));
}

void UpdateScanResults() {
  std::map<Gadget, std::vector<Sample>> scanResults;
  sensiScan.getScanResults(scanResults);
  for (const auto& item : scanResults) {
    Gadget gadget = item.first;
    //Serial.printf("- %s Gadget with ID: %s \n", gadget.name.c_str(), gadget.deviceId.c_str());
    for (int i = 0; i < item.second.size(); i++) {
      Sample sample = item.second[i];
      //Serial.printf("  - %s -> %f \n", unitTypeString[sample.type].c_str(), sample.value);
      if (sample.type == UnitType::CO2) {
        co2_samples[gadget] = sample;
      } else if (sample.type == UnitType::RH) {
        humidity_samples[gadget] = sample;
      } else if (sample.type == UnitType::T) {
        temperature_samples[gadget] = sample;
      } else if (sample.type == UnitType::VOC) {
        voc_samples[gadget] = sample;
      }
    }
  }
}

const char* getCurrentUnit() {
  if (current_unit == UnitType::CO2) {
    return "CO2";
  }
  if (current_unit == UnitType::RH) {
    return "Humidity";
  }
  if (current_unit == UnitType::T) {
    return "Temperature";
  }
  if (current_unit == UnitType::VOC) {
    return "VOC Index";
  }  
}

void selectAndDisplaySample() {
  std::map<Gadget, Sample> current_samples = getCurrentSamples();
  if (current_samples.empty()) {
    // nothing to display
    displaySplashScreen();
    return;
  }

  // gadget not available or dummy gadget
  if (current_samples.count(current_gadget) == 0) {
    // select first available gadget
    current_gadget = current_samples.begin()->first;
  }

  Sample current_sample = current_samples[current_gadget];
  Serial.printf("Current sample of type %s from %s => %f\n", getCurrentUnit(), current_gadget.deviceId.c_str(), current_sample.value);
  
  switch (current_unit) {
    case UnitType::RH:
      displayRelativeHumidity(current_sample.value, current_gadget.deviceId.c_str());
      break;
    case UnitType::T:
      displayTemperature(current_sample.value, current_gadget.deviceId.c_str());
      break;
    case UnitType::VOC:
      displayVoc(current_sample.value, current_gadget.deviceId.c_str());
      break;
    case UnitType::CO2:
      displayCo2(current_sample.value, current_gadget.deviceId.c_str());
      break;
  }
}

void rotateSelectedUnit() {
  if (current_unit == UnitType::CO2) {
    current_unit = UnitType::RH;
    return;
  }

  if (current_unit == UnitType::RH) {
    current_unit = UnitType::T;
    return;
  }
 
  if (current_unit == UnitType::T) {
    current_unit = UnitType::VOC;
    return;
  }

  if (current_unit == UnitType::VOC) {
    current_unit = UnitType::CO2;
    return;
  }
}

void rotateSelectedGadget() {
  std::map<Gadget, Sample> current_samples = getCurrentSamples();
  if(current_samples.size() <= 1) {
    // nothing to rotate through
    return;
  }

  bool take_next = false;
  
  for (const auto entry: current_samples) {
    if (take_next) {
      current_gadget = entry.first;
      return;
    }
    if (entry.first.deviceId == current_gadget.deviceId) {
      take_next = true;
    }
  }

  // we reached end of list, take first again
  if (take_next) {
    current_gadget = current_samples.begin()->first;
  }
}

void setupButtons() {
  btn_left.setPressedHandler([](Button2 & b) {
    Serial.printf("Left button pressed\n");
    rotateSelectedUnit();
    selectAndDisplaySample();
  });
  
  btn_right.setPressedHandler([](Button2 & b) {
    Serial.printf("Right button pressed\n");
    rotateSelectedGadget();
    selectAndDisplaySample();
  });
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
      delay(100);
  }

  setupDisplay();
  setupButtons();
  
  displaySplashScreen();

  sensiScan.begin();
}

void loop() {
  buttonLoop();
  if (millis() - lastUpdateTimeMs >= updateIntervalMs) {
    UpdateScanResults();
    selectAndDisplaySample();
    lastUpdateTimeMs = millis();
  }
}
