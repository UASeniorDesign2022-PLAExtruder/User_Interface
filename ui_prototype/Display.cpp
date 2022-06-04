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

void Display::direct_I2C_Numeric_Param(uint8_t data_ID, float value)
{
    bool is_status_input = false;
    if (data_ID > 0x0F)
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
            ID = 10;
            break;
        case 0x0C:
            ID = 11;
            break;
        case 0x0D:
            ID = 12;
        default:
            break;
    }
    set_new_numeric_value(value, ID);
}




/*____Status___________I2C Code__*/
/*  STATUS_NONE          0x00    */
/*  STATUS_READY         0x01    */                 
/*  STATUS_ON            0x02    */
/*  STATUS_OFF           0x03    */
/*  STATUS_OPEN          0x04    */
/*  STATUS_CLOSED        0x05    */
/*  STATUS_IN_PROGRESS   0x06    */
/*  STATUS_COMPLETE      0x07    */



        
void Display::direct_I2C_Status_Param(uint8_t data_ID, uint8_t status_ID)
{
    unsigned char ID = 0;
    char* status_value;
    switch(data_ID)
    {
        case 0x10:
            ID = 0;
            break;
        case 0x20:
            ID = 1;
            break;
        case 0x30:
            ID = 2;
            break;
        case 0x40:
            ID = 3;
            break;
        default:
            break;
    }

    switch(status_ID)
    {
        case 0x00:
            status_value = STATUS_NONE;
            break;
        case 0x01:
            status_value = STATUS_READY;
            break;
        case 0x02:
            status_value = STATUS_ON;
            break;
        case 0x03:
            status_value = STATUS_OFF;
            break;
        case 0x04:
            status_value = STATUS_OPEN;
            break;
        case 0x05:
            status_value = STATUS_CLOSED;
            break;
        case 0x06:
            status_value = STATUS_IN_PROGRESS;
            break;
        case 0x07:
            status_value = STATUS_COMPLETE;
            break;
        default:
            break;
    }
    set_new_status_value(status_value, ID);
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
    if (new_value != numeric_params[ID]->VALUE)
    {
        numeric_params[ID]->VALUE = new_value;
        numeric_params[ID]->IS_CURRENT = false;
    }
}
/* END set_new_numeric_value() */

/* set_new_status_value() */
void Display::set_new_status_value(char* new_value, unsigned char ID)
{
    if (new_value != status_params[ID]->VALUE)
    {
        status_params[ID]->VALUE = new_value;
        status_params[ID]->IS_CURRENT = false;
    }
}
/* END set_new_status_value() */