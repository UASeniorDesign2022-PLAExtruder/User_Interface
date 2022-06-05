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

void Display::direct_I2C_Numeric_Param(uint8_t global_id, float incoming_numeric_value)
{
    set_new_numeric_value(incoming_numeric_value, global_id - 1);
}

/* set_text() */
void Display::set_text(uint8_t text_size, uint16_t text_color)
{
    tft.setTextSize(text_size);
    tft.setTextColor(text_color);
}
/* END set_text() */

/* set_default_background() */
void Display::set_default_background()
{
    tft.fillScreen(HX8357_BLACK);
    set_text(2, HX8357_MAGENTA);
    tft.setCursor(0, 0);
    tft.print("PLA Filament Extruder");
}
/* END set_default_background() */

/* set_output_screen() */
void Display::set_output_screen()
{
    set_default_background();
    set_label_and_value(numeric_params, NUMERIC_T);
//    set_label_and_value(status_params, STATUS_T);
}
/* END set_output_screen() */

/* set_new_numeric_value() */
void Display::set_new_numeric_value(float new_value, uint8_t local_id)
{
    if (new_value != numeric_params[local_id]->value)
    {
        numeric_params[local_id]->value = new_value;
        numeric_params[local_id]->is_current = false;
    }
}
/* END set_new_numeric_value() */
