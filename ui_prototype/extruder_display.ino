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

String data_string = "";
char data_source_id = 0;

void setup()
{
    Serial.begin(9600);
    Wire.begin(0x14);
    /*Event Handlers*/
    Wire.onReceive(data_receive);
    Wire.onRequest(data_request);
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

void data_receive()
{
    unsigned char count = 0;
    while(Wire.available()) 
    { 
        char receive_data = Wire.read();
        if (count == 0)
            data_source_id = receive_data;
        else
            data_string = data_string + receive_data;
        count++;
    }
    float data_string_value = data_string.toFloat();
    Serial.print("data source  ID:\t"); Serial.println(data_source_id);
    Serial.print("data string val:\t"); Serial.println(data_string_value);
}

void data_request()
{
    byte slave_send = 0xFF;
    Wire.write(slave_send);
}
