/*********************************************************************
 * PLA/PET Filament Extruder
 * Wilson Woods
 * 10.22.2021
 * display_functions.h
 * User Interface Application
 *    Arduino Uno
 *    Adafruit HX8357
 ***********************************************************************/
#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include "Arduino.h"
#include "Adafruit_HX8357.h"
#include "Adafruit_GFX.h"
#include "TouchScreen.h"

#define TFT_CS              10
#define TFT_DC              9
#define TFT_RST             8
#define YP                  A2
#define XM                  A3
#define YM                  7
#define XP                  8

#define NUMERIC_T           0
#define STATUS_T            1

#define RIGHT_COLUMN        0
#define LEFT_COLUMN         1

#define LABEL_CURSOR        0
#define VALUE_CURSOR        250

#define ROW_HEIGHT          25

#define FILL_RECT_WIDTH     100
#define FILL_RECT_HEIGHT    ROW_HEIGHT
#define HEADER_OFFSET       35

const uint8_t NUMERIC_PARAM_COUNT = 9;

const uint8_t ZONE_1_TEMP_GLOBAL_ID             = 0x01;
const uint8_t HEATER_1_POWER_GLOBAL_ID          = 0x02;
const uint8_t ZONE_2_TEMP_GLOBAL_ID             = 0x03;
const uint8_t HEATER_2_POWER_GLOBAL_ID          = 0x04;
const uint8_t ZONE_3_TEMP_GLOBAL_ID             = 0x05;
const uint8_t HEATER_3_POWER_GLOBAL_ID          = 0x06;
const uint8_t SCREW_SPEED_GLOBAL_ID             = 0x07;
const uint8_t ROLLER_SPEED_GLOBAL_ID            = 0x08;
const uint8_t SPOOLER_SPEED_GLOBAL_ID           = 0x09;

class Display
{
    public:
        /* hardware interface objects */
        Adafruit_HX8357 tft = Adafruit_HX8357( TFT_CS, TFT_DC, TFT_RST );
        TouchScreen ts = TouchScreen( XP, YP, XM, YM, 285 );

        struct Numeric_Param
        {
            const uint8_t global_id;
            const char* label;
            float value;
            bool is_current;
            uint8_t row;
        };  
        
        struct X_Y
        {
            float x = 0.0;
            float y = 0.0;
        };    
         
        /*******************************************************************************************************************/
        
        X_Y pr;
        /*                                  GLOBAL ID                     LABEL                        VALUE        IS CURRENT    ROW */
        Numeric_Param zone_1_temp       = { ZONE_1_TEMP_GLOBAL_ID,        "Zone 1 Temp. (C): ",        0.0,         true,         0 };
        Numeric_Param heater_1_power    = { HEATER_1_POWER_GLOBAL_ID,     "Zone 1 Power (%): ",        0.0,         true,         0 };
        Numeric_Param zone_2_temp       = { ZONE_2_TEMP_GLOBAL_ID,        "Zone 2 Temp. (C): ",        0.0,         true,         0 };
        Numeric_Param heater_2_power    = { HEATER_2_POWER_GLOBAL_ID,     "Zone 2 Power (%): ",        0.0,         true,         0 };
        Numeric_Param zone_3_temp       = { ZONE_3_TEMP_GLOBAL_ID,        "Zone 3 Temp. (C): ",        0.0,         true,         0 };
        Numeric_Param heater_3_power    = { HEATER_3_POWER_GLOBAL_ID,     "Zone 3 Power (%): ",        0.0,         true,         0 };
        Numeric_Param screw_speed       = { SCREW_SPEED_GLOBAL_ID,        "Screw Speed: ",             0.0,         true,         0 };
        Numeric_Param roller_speed      = { ROLLER_SPEED_GLOBAL_ID,       "Roller Speed: ",            0.0,         true,         0 };
        Numeric_Param spooler_speed     = { SPOOLER_SPEED_GLOBAL_ID,      "Spooler Speed: ",           0.0,         true,         0 };
        
        /*******************************************************************************************************************/
        Numeric_Param* numeric_params[NUMERIC_PARAM_COUNT] = { &zone_1_temp, &heater_1_power, &zone_2_temp, &heater_2_power, &zone_3_temp, &heater_3_power,
                                                               &screw_speed, &roller_speed, &spooler_speed };
                                                                                                               
        /*******************************************************************************************************************/
        template <class T> void set_label_and_value(T Params_Array, uint8_t label_type);
        template <class T> void update_output(T Params_Array, uint8_t local_id);
        template <class T> void poll_inputs(T Params_Array, uint8_t param_array_size);
        template <class T> void set_numeric_input_screen(T Params_Array, const uint8_t local_id);
        template <class T> void get_numeric_user_input(T Params_Array, const uint8_t local_id); 
        void direct_I2C_Numeric_Param(uint8_t global_id, float incoming_numeric_value); 
//        void direct_I2C_Status_Param(uint8_t global_id, uint8_t incoming_status); 
        void set_text(uint8_t text_size, uint16_t text_color);
        void set_new_numeric_value(float new_value, uint8_t local_id);
//        void set_new_status_value(char* new_value, uint8_t local_id);
        void set_default_background();
        void set_output_screen();
};

/* set_label_and_value() */
template <class T> void Display::set_label_and_value (T Params_Array, uint8_t label_type)
{
    for (uint8_t index = 0; index < NUMERIC_PARAM_COUNT; ++index)
    {
        set_text(2, HX8357_CYAN);

        uint16_t row_position = 0;
        
        if (index < 6)
            row_position = (index % 2 != 0 ? (index * ROW_HEIGHT) + HEADER_OFFSET + 5 : (index * ROW_HEIGHT) + HEADER_OFFSET + 15);
        else
            row_position = (index * ROW_HEIGHT) + HEADER_OFFSET + 20;
        
        Params_Array[index]->row = row_position;

        tft.setCursor(LABEL_CURSOR, Params_Array[index]->row);
        tft.print(Params_Array[index]->label);

        tft.setCursor(VALUE_CURSOR, Params_Array[index]->row);
        tft.print(Params_Array[index]->value); 
    }
}

/* END set_label_and_value() */

/* update_output() */
template <class T> void Display::update_output( T Params_Array, uint8_t local_id )
{
    set_text(1, HX8357_WHITE);
    tft.fillRect(VALUE_CURSOR, Params_Array[local_id]->row, FILL_RECT_WIDTH, FILL_RECT_HEIGHT, HX8357_BLACK);
    tft.setCursor(VALUE_CURSOR, Params_Array[local_id]->row);
    tft.print(Params_Array[local_id]->value);
    Params_Array[local_id]->is_current = true;
}
/* END update_output() */

/* poll_inputs() */
template <class T> void Display::poll_inputs(T Params_Array, uint8_t param_array_size)
{
    for (uint8_t local_id = 0; local_id < param_array_size; local_id++)
        if (Params_Array[local_id]->is_current == false) { update_output(Params_Array, local_id); }
}
/* END poll_inputs() */

      
#endif
