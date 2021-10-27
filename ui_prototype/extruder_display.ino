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
#include <Wire.h>
#include <stdint.h>
#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"
#include "TouchScreen.h"
#include "Display.h"

Display UI;


char data_source_id = 0;

union floatToBytes
{
    char buffer[4];
    float numeric_param_input;
};

union floatToBytes converter;
byte status_param_input = 0;

void setup()
{

    Wire.setClock(400000);
    Wire.begin(0x14);
    
    Wire.onReceive(I2C_receive_event);
    Serial.begin(9600);
    byte complete = 0x07;
    /*Event Handlers*/
    UI.tft.begin();
    UI.tft.setRotation(1);
    UI.set_numeric_input_screen(UI.numeric_params, UI.desired_yield.ID);
    UI.get_numeric_user_input(UI.numeric_params, UI.desired_yield.ID);
    UI.required_input.VALUE = UI.desired_yield.VALUE * 1.11;
    delay(500);
    UI.set_output_screen();
    UI.set_new_numeric_value(999.9, 4);
    UI.set_new_numeric_value(999.9, 5);
    UI.set_new_numeric_value(999.9, 6);
    UI.set_new_status_value("In Progress", 2);
}

void loop()
{
    UI.poll_inputs(UI.numeric_params, NUMERIC_PARAM_COUNT);
    UI.poll_inputs(UI.status_params, STATUS_PARAM_COUNT);
    delay(1000);
}

void I2C_receive_event()
{
    uint8_t index = 0;
    byte incoming_data_ID;
    while (Wire.available())
    {
        if (index == 0)
        {
            incoming_data_ID = Wire.read();
            index++;
            continue;
        }
        if (incoming_data_ID < 5)
            status_param_input = Wire.read(); 
        else
            converter.buffer[index - 1] = Wire.read();
        index++;    
    }
    Serial.print("data ID: "); Serial.print(incoming_data_ID); Serial.print("\tdata: ");
    if (incoming_data_ID < 5) { Serial.println(status_param_input); }
    else                      { Serial.println(converter.numeric_param_input); }
}
