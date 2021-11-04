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

void Display::direct_I2C_data(uint8_t data_ID, uint8_t data)
{
    bool is_status_input = false;
    if (data_ID > 0x0A)
        is_status_input = true;
    unsigned char ID = 0;
    switch(data_ID)
    {
        case 0x01:
            ID = 0;
            break;  
        case 0x02:
            ID = 1;
            break;
        case 0x03:
            ID = 2;
            break;
        case 0x04:
            ID = 3;
            break;
        case 0x05:
            ID = 4;
            break;
        case 0x06:
            ID = 5;
            break;
        case 0x07:
            ID = 6;
            break;
        case 0x08:
            ID = 7;
            break;
        case 0x09:
            ID = 8;
            break;
        case 0x0A:
            ID = 9;
            break;
        case 0x0B:
            ID = 0;
            break;
        case 0x0C:
            ID = 1;
            break;
        case 0x0D:
            ID = 2;
            break;
        case 0x0E:
            ID = 3;
            break;
        default:
            break;
    }
    if (is_status_input == false)
        set_new_numeric_value((float)data, ID);
    else
        return;
        // set_new_status_value((String)data, ID);
    
}

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

/* set_new_numeric_value() */
void Display::set_new_numeric_value(float new_value, unsigned char ID)
{
    numeric_params[ID]->VALUE = new_value;
    numeric_params[ID]->IS_CURRENT = false;
}
/* END set_new_numeric_value() */

/* set_new_status_value() */
void Display::set_new_status_value(char* new_value, unsigned char ID)
{
    status_params[ID]->VALUE = new_value;
    status_params[ID]->IS_CURRENT = false;
}
/* END set_new_status_value() */
