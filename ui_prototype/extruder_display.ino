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
    char buffer[4];
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
    uint8_t incoming_value = 0;
};

I2C_INT_DATA i2c_int_data;


/*
uint8_t ack = 0;
bool I2C_message_initiated = false;
bool I2C_receive_message = false;
*/

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
    // delay(500);
    // UI.set_new_numeric_value(999.9, 4);
    // UI.set_new_numeric_value(999.9, 5);
    // UI.set_new_numeric_value(999.9, 6);
    // UI.set_new_status_value("In Progress", 2);
}

void loop()
{

   // Wire.write(outgoing_message);
   /*
   if(Wire.available())
   {
      status_param_input = Wire.read();
   }
   */
    if(i2c_int_data.incoming_ID != 0)
    {
        UI.direct_I2C_data(i2c_int_data.incoming_ID, i2c_int_data.incoming_value);
        UI.poll_inputs(UI.numeric_params, NUMERIC_PARAM_COUNT);
        i2c_int_data.incoming_ID = 0;
        i2c_int_data.incoming_value = 0;
    }
    //UI.poll_inputs(UI.status_params, STATUS_PARAM_COUNT);
    /*
    if (I2C_message_initiated == true)
    {
        Wire.write(255);
        I2C_message_initiated = false;
    }
    */
    
    // Serial.print("data ID: "); Serial.print(incoming_data_ID); Serial.print("\tdata: ");
    
    /*
    incoming_data_ID = 0;
    status_param_input = 0;s
    incoming_message = 0;
    */
}


void I2C_receive_event(int howMany)
{

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
            i2c_int_data.incoming_value = Wire.read();
        }
        index++;
    }
    // if (incoming_data_ID < 5) { Serial.println(status_param_input); }
    // else                      { Serial.println(converter.numeric_param_input); }
}

void I2C_request_event()
{
  
    Wire.write(0x14);
}
