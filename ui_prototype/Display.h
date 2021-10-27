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

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8
#define YP A2
#define XM A3
#define YM 7
#define XP 8

const unsigned char NUMERIC_PARAM_COUNT = 10;
const unsigned char STATUS_PARAM_COUNT = 4;

class Display
{
    public:
        /* hardware interface objects */
        Adafruit_HX8357 tft = Adafruit_HX8357( TFT_CS, TFT_DC, TFT_RST );
        TouchScreen ts = TouchScreen( XP, YP, XM, YM, 285 );

        struct Numeric_Param
        {
            const char* LABEL;
            const unsigned char ID;
            float VALUE;
            bool IS_CURRENT;
            const char  COLUMN;
            const unsigned char ROW;
        };  
        struct Status_Param
        {
            const char* LABEL;
            const unsigned char ID;
            char* VALUE;
            bool IS_CURRENT;
            const char  COLUMN;
            const unsigned char ROW;
        };
        struct X_Y
        {
            float x = 0.0;
            float y = 0.0;
        };                                                               /*____Status___________I2C Code__*/
        const char* STATUS_NONE         = "--";                          /*  STATUS_NONE          0x00    */
        const char* STATUS_READY        = "Ready";                       /*  STATUS_READY         0x01    */                 
        const char* STATUS_ON           = "ON";                          /*  STATUS_ON            0x02    */
        const char* STATUS_OFF          = "OFF";                         /*  STATUS_OFF           0x03    */
        const char* STATUS_OPEN         = "Open";                        /*  STATUS_OPEN          0x04    */
        const char* STATUS_CLOSED       = "Closed";                      /*  STATUS_CLOSED        0x05    */
        const char* STATUS_IN_PROGRESS  = "In Progress";                 /*  STATUS_IN_PROGRESS   0x06    */
        const char* STATUS_COMPLETE     = "Complete";                    /*  STATUS_COMPLETE      0x07    */
        /*******************************************************************************************************************/
        X_Y pr;
        Numeric_Param desired_yield     = { "Desired Yield (kg): ",     0, 0.0,         1, 'L', 60  }; // col L row 3
        Numeric_Param required_input    = { "Required Input (kg): ",    1, 0.0,         1, 'L', 80  }; // col L row 4
        Numeric_Param ground_weight     = { "Ground Weight (kg): ",     2, 0.0,         1, 'L', 100 }; // col L row 5
        Numeric_Param zone_1_temp       = { "Zone 1: ",                 3, 0.0,         1, 'R', 100 }; // col R row 5
        Numeric_Param zone_2_temp       = { "Zone 2: ",                 4, 0.0,         1, 'R', 120 }; // col R row 6
        Numeric_Param zone_3_temp       = { "Zone 3: ",                 5, 0.0,         1, 'R', 140 }; // col R row 7
        Numeric_Param screw_speed       = { "Screw Speed [RPM]: ",      6, 0.0,         1, 'R', 160 }; // col R row 10
        Numeric_Param filament_diameter = { "Diameter (mm): ",          7, 0.0,         1, 'R', 180 }; // col R row 11
        Numeric_Param extruded_length   = { "Extruded Length (m): ",    8, 0.0,         1, 'R', 200 }; // col R row 12
        Numeric_Param projected_yield   = { "Projected Yield (kg): ",   9, 0.0,         1, 'R', 60  }; // col R row 3
        Status_Param hopper_lid_status  = { "Hopper Lid Status: ",      0, STATUS_NONE, 1, 'L', 120 }; // col L row 6
        Status_Param grinder_status     = { "Grinder (On/Off): ",       1, STATUS_NONE, 1, 'L', 140 }; // col L row 7
        Status_Param preparation_status = { "Status: ",                 2, STATUS_NONE, 1, 'L', 40  }; // col L row 2
        Status_Param extrusion_status   = { "Status: ",                 3, STATUS_NONE, 1, 'R', 40  }; // col R row 2
        /*******************************************************************************************************************/
        Numeric_Param* numeric_params[NUMERIC_PARAM_COUNT] = { &desired_yield, &required_input, &ground_weight,
                                                               &zone_1_temp, &zone_2_temp, &zone_3_temp,
                                                               &screw_speed, &filament_diameter, &extruded_length,
                                                               &projected_yield };                                                  
        Status_Param* status_params[STATUS_PARAM_COUNT]    = { &hopper_lid_status, &grinder_status, &preparation_status,
                                                               &extrusion_status };
        /*******************************************************************************************************************/
        template <class T> void set_label_and_value(T Params_Array, unsigned char label_type);
        template <class T> void update_output(T Params_Array, unsigned char ID);
        template <class T> void poll_inputs(T Params_Array, unsigned char SIZE);  
        template <class T> void set_numeric_input_screen(T Params_Array, const unsigned char ID);
        template <class T> void get_numeric_user_input(T Params_Array, const unsigned char ID);   
        void set_text(unsigned char S, unsigned short C);
        void set_new_numeric_value(float new_value, unsigned char ID);
        void set_new_status_value(char* new_value, unsigned char ID);
        void set_default_background();
        void set_output_screen();
};

/* set_label_and_value() */
template <class T> void Display::set_label_and_value (T Params_Array, unsigned char label_type)
{
    unsigned short text_color = 0;
    unsigned char item_count = 0;
    unsigned short label_cursor = 0;
    unsigned short value_cursor = 130;

    set_text(1, HX8357_CYAN);
    tft.setCursor(0, 20);
    tft.print("Preparation Stage");
    tft.setCursor(235, 20);
    tft.print("Extrusion Stage");
    tft.setCursor(235, 80);
    tft.print("Temperatures");
    if (label_type == 0) { item_count = NUMERIC_PARAM_COUNT; }
    else { item_count = STATUS_PARAM_COUNT; }
    set_text(1,HX8357_WHITE);
    for (int ID = 0; ID < item_count; ID++)
    {
        if (Params_Array[ID]->COLUMN == 'R') { label_cursor = 235; value_cursor = 385; }
        tft.setCursor(label_cursor, Params_Array[ID]->ROW);
        tft.print(Params_Array[ID]->LABEL);
        tft.setCursor(value_cursor, Params_Array[ID]->ROW);
        tft.print(Params_Array[ID]->VALUE);
    }
}
/* END set_label_and_value() */

/* update_output() */
template <class T> void Display::update_output( T Params_Array, unsigned char ID )
{
    unsigned short value_cursor = 130;
    if (Params_Array[ID]->COLUMN == 'R') { value_cursor = 385; }
    set_text(1, HX8357_WHITE);
    tft.fillRect(value_cursor, Params_Array[ID]->ROW, 95, 15, HX8357_BLACK);
    tft.setCursor(value_cursor, Params_Array[ID]->ROW);
    tft.print(Params_Array[ID]->VALUE);
    Params_Array[ID]->IS_CURRENT = true;
}
/* END update_output() */

/* poll_inputs() */
template <class T> void Display::poll_inputs(T Params_Array, unsigned char SIZE)
{
    for (int ID = 0; ID < SIZE; ID++)
        if (Params_Array[ID]->IS_CURRENT == false) { update_output(Params_Array, ID); }
}
/* END poll_inputs() */

/* set_numeric_input_screen() */
template <class T> void Display::set_numeric_input_screen(T Params_Array, const unsigned char ID)
{
    unsigned short x_cursor = 42;
    unsigned char row_1_item, row_2_item;
    unsigned short bottom_label_positions[] = { 5, 115, 235, 310 };
    char *bottom_labels[]                   = { "Go Back", "Clear", ".", "Enter" };
    set_default_background();
    tft.setCursor(0, 20);
    set_text(2, HX8357_WHITE);
    tft.print(Params_Array[ID]->LABEL);
    // horizontal lines
    for (unsigned short n = 0; n < 3; n++)
        tft.drawLine(0, 50 + (n * 90), 480, 50 + (n * 90), HX8357_WHITE);
    for (unsigned char n = 0; n < 4; n++)
        tft.drawLine(96 * (n + 1), 50, 96 * (n + 1), 320, HX8357_WHITE);
    for (unsigned char i = 0; i < 5; i++)
    {
        tft.setCursor(x_cursor, 85);
        tft.print(i + 1);
        tft.setCursor(x_cursor, 175);
        if (i != 4) { tft.print(i + 6); }
        else { tft.print(0); }
        x_cursor += 96;
    }
    for (unsigned char i = 0; i < 4; i++)
    {
        tft.setCursor(bottom_label_positions[i], 265);
        tft.print(bottom_labels[i]);
    }
}
/* END set_numeric_input_screen() */

/* get_numeric_user_input() */
template <class T> void Display::get_numeric_user_input(T Params_Array, const unsigned char ID)
{
    unsigned char row = 0;
    unsigned char col = 0;
    unsigned char index = 0;
    unsigned char decimal_index = 0;
    unsigned short sum = 0;
    float input_value = 0.0;
    char button = 'n';
    const char *buttons[] = { "12345", "67890", "bc.en" };
    unsigned short text_box_cursor_x = 230;
    set_text(2, HX8357_WHITE);

    while (1)
    {
        TSPoint p = ts.getPoint();
        delay(300);
        if (p.z > 3)
        {
            pr.x = (float)p.y * (480.0 / 1023.0);
            pr.y = 320.0 - ((float)p.x * (320.0 / 1023.0));
            if      (pr.x < (96 - 2)) { col = 0; }
            else if (pr.x > (96 + 2) && pr.x < ((96 * 2) - 2)) { col = 1; }
            else if (pr.x > ((96 * 2) + 2) && pr.x < ((96 * 3) - 2)) { col = 2; }
            else if (pr.x > ((96 * 3) + 2) && pr.x < ((96 * 4) - 2)) { col = 3; }
            else if (pr.x > ((96 * 4) + 2) && pr.x < 480) { col = 4; }
            else    { col = 1000; }
            if      (pr.y > (50 + 2)  && pr.y < (50 + 90 - 2)) { row = 0; }
            else if (pr.y > (50 + 90 + 2) && pr.y < (50 + (90 * 2) - 2)) { row = 1; }
            else if (pr.y > (50 + (90 * 2) + 2) && pr.y < 320) { row = 2; }
            else    { row = 1000; }
            pr.x = 1000;
            pr.y = 1000;
            button = buttons[row][col];
            if (isDigit(button) || button == '.')
            {
                text_box_cursor_x += 12;
                tft.setCursor(text_box_cursor_x, 20);
                tft.print(button);
                if (isDigit(button))
                    sum = (sum * 10) + (button - '0');
                else
                    decimal_index = index;
                index++;
            }
            else if (button == 'b')
                return 0.0;
            else if (button == 'c')
            {
                tft.fillRect(230, 20, 300, 20, HX8357_BLACK);
                text_box_cursor_x = 230;
                button = 'n';
                sum = 0;
                index = 0;
            }
            else if (button == 'e')
                break;
        }
    }
    input_value = sum / pow(10, (index - decimal_index - 1));
    set_new_numeric_value(input_value, ID);
}
/* END get_numeric_user_input() */
      
#endif
