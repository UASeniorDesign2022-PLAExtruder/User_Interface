 /***********************************************************************
 * PLA/PET Filament Extruder
 * Wilson Woods
 * 10.22.2021
 * extruder_display.ino
 * User Interface Application
 *    Arduino Uno
 *    Adafruit HX8357
 ***********************************************************************/
#include <SPI.h>
#include <stdint.h>
#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"
#include "TouchScreen.h"
#include "Display.h"

Display UI;

void setup()
{
    Serial.begin(9600);
    Serial.println("HX8357D Test!");
    UI.tft.begin();
    // read diagnostics (optional but can help debug problems)
    uint8_t x = UI.tft.readcommand8(HX8357_RDPOWMODE);
    Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
    x = UI.tft.readcommand8(HX8357_RDMADCTL);
    Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
    x = UI.tft.readcommand8(HX8357_RDCOLMOD);
    Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
    x = UI.tft.readcommand8(HX8357_RDDIM);
    Serial.print("Image Format: 0x"); Serial.println(x, HEX);
    x = UI.tft.readcommand8(HX8357_RDDSDR);
    Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
    Serial.println(F("Benchmark Time (microseconds)"));
    Serial.println(F("Done!"));
  
    UI.tft.setRotation(1);
    UI.tft.fillScreen(HX8357_BLACK);
    UI.set_numeric_input_screen();
    UI.get_numeric_user_input();
    UI.tft.fillScreen(HX8357_BLACK);
    UI.set_output_screen();
}

void loop()
{
    UI.poll_inputs(UI.numeric_params, NUMERIC_PARAM_COUNT);
    UI.poll_inputs(UI.status_params, STATUS_PARAM_COUNT);
    delay(1000);
}
