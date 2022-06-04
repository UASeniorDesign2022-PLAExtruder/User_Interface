   /***********************************************************************
 * PLA/PET Filament Extruder
 * Wilson Woods
 * 10.22.2021
 * extruder_display.ino
 * User Interface Application
 *    Arduino Uno
 *    Adafruit HX8357
 *    CURRENT
 ***********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <stdint.h>
#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"
#include "TouchScreen.h"
#include "Display.h"

Display UI;

union floatToBytes
{
    uint8_t buffer[4];
    float numeric_param_input;
};

union floatToBytes converter;
/*
uint8_t incoming_message = 0;
uint8_t outgoing_message = 0x55;
*/
uint8_t incoming_data_ID = 0;
uint8_t status_param_input = 0;

struct I2C_INT_DATA
{
    uint8_t incoming_ID = 0;
    uint8_t incoming_status = 0;
    float incoming_value = 0;
};

I2C_INT_DATA i2c_int_data;

bool received_message = false;
bool is_status = false;

void setup()
{
    Wire.begin(0x14);
    
    /*Event Handlers*/
    Wire.onReceive(I2C_receive_event);
    Wire.onRequest(I2C_request_event);
    Serial.begin(9600);
    byte complete = 0x07;
    UI.tft.begin();
    UI.tft.setRotation(1);
    // UI.set_numeric_input_screen(UI.numeric_params, UI.desired_yield.ID);
    // UI.get_numeric_user_input(UI.numeric_params, UI.desired_yield.ID);
    // UI.required_input.VALUE = UI.desired_yield.VALUE * 1.11;
    
    UI.set_output_screen();
}

void loop()
{
    if(received_message == true)
    {
        if (is_status == true)
        {
            UI.direct_I2C_Status_Param(i2c_int_data.incoming_ID, i2c_int_data.incoming_status);
        }
        else
        {
            UI.direct_I2C_Numeric_Param(i2c_int_data.incoming_ID, i2c_int_data.incoming_value);
        }
        i2c_int_data.incoming_ID = 0;
        i2c_int_data.incoming_status = 0;
        i2c_int_data.incoming_value = 0;
        received_message = false;
        is_status = false;    
    }
    
    UI.poll_inputs(UI.numeric_params, NUMERIC_PARAM_COUNT);
    UI.poll_inputs(UI.status_params, STATUS_PARAM_COUNT);
}


void I2C_receive_event(int howMany)
{TMCM-1633
    
    uint8_t index = 0;
    while (Wire.available())
    {
        if (index == 0)
        {
            i2c_int_data.incoming_ID = Wire.read();
            index++;
            continue;
        }
        else
        {
            if (i2c_int_data.incoming_ID > 0x0F)
            {
                i2c_int_data.incoming_status = Wire.read();
                is_status = true;
            }
            else
            {
                converter.buffer[index - 1] = Wire.read();
            }
            
        }
        index++;
    }
    received_message = true;
    i2c_int_data.incoming_value = converter.numeric_param_input;
    converter.numeric_param_input = 0;
}

void I2C_request_event()
{
  
    Wire.write(0x14);
}
