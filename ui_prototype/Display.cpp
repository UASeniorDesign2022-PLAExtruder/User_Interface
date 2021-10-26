/***********************************************************************
 * PLA/PET Filament Extruder
 * Wilson Woods
 * 10.22.2021
 * display_functions.cpp
 * User Interface Application
 *    Arduino Uno
 *    Adafruit HX8357
 ***********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <math.h>
#include "Arduino.h"
#include "Adafruit_HX8357.h"
#include "Adafruit_GFX.h"
#include "TouchScreen.h"
#include "Display.h"

/* set_text() */
void Display::set_text(unsigned char S, unsigned short C)
{
    tft.setTextSize(S);
    tft.setTextColor(C);
}
/* END set_text() */

/* set_default_background() */
void Display::set_default_background()
{
    tft.fillScreen(HX8357_BLACK);
    set_text(2, HX8357_MAGENTA);
    tft.setCursor(0, 0);
    tft.print("PLA/PET Filament Extruder");
}
/* END set_default_background() */

/* set_output_screen() */
void Display::set_output_screen()
{
    set_default_background();
    tft.drawLine(230, 20, 230, 320, HX8357_WHITE);
    set_label_and_value(numeric_params, 0);
    set_label_and_value(status_params, 1);
}
/* END set_output_screen() */

void Display::set_new_numeric_value(float new_value, unsigned char ID)
{
    numeric_params[ID]->VALUE = new_value;
    numeric_params[ID]->IS_CURRENT = false;
}

void Display::set_new_status_value(char* new_value, unsigned char ID)
{
    status_params[ID]->VALUE = new_value;
    status_params[ID]->IS_CURRENT = false;
}
