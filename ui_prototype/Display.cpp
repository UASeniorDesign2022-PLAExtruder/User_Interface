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
#include "Arduino.h"
#include "Adafruit_HX8357.h"
#include "Adafruit_GFX.h"
#include "TouchScreen.h"
#include "Display.h"

/* set_output_screen() */
unsigned long Display::set_output_screen()
{
    unsigned long start = micros();
    tft.drawLine(DIVIDER_LINE_CURSOR_X, Y_MIN, DIVIDER_LINE_CURSOR_X, Y_MAX, HX8357_WHITE);
    tft.setTextSize(2);
    tft.setCursor(title_heading.LABEL_CURSOR_X, title_heading.CURSOR_Y);
    tft.setTextColor(HX8357_BLUE);
    tft.print(title_heading.LABEL);
    tft.setTextSize(1);
    tft.setTextColor(HX8357_WHITE);
    for (int ID = 1; ID < HEADING_COUNT; ID++)        { set_heading(ID); }
    for (int ID = 0; ID < NUMERIC_PARAM_COUNT; ID++)  { set_numeric_label_and_value(ID); }
    for (int ID = 0; ID < STATUS_PARAM_COUNT; ID++)   { set_status_label_and_value(ID); }
    return micros() - start;
}
/* END set_output_screen() */

/* set_numeric_label_and_value() */
unsigned long Display::set_numeric_label_and_value(unsigned char ID)
{
    unsigned long start = micros();
    tft.setTextSize(1);
    tft.setTextColor(HX8357_WHITE);
    tft.setCursor(numeric_params[ID]->LABEL_CURSOR_X, numeric_params[ID]->CURSOR_Y);
    tft.print(numeric_params[ID]->LABEL);
    tft.setCursor(numeric_params[ID]->VALUE_CURSOR_X, numeric_params[ID]->CURSOR_Y);
    tft.print(numeric_params[ID]->CURRENT_VALUE);
    return micros() - start;
}
/* END set_numeric_label_and_value() */

/* set_status_label_and_value() */
unsigned long Display::set_status_label_and_value(unsigned char ID)
{
    unsigned long start = micros();
    tft.setTextSize(1);
    tft.setTextColor(HX8357_WHITE);
    tft.setCursor(status_params[ID]->LABEL_CURSOR_X, status_params[ID]->CURSOR_Y);
    tft.print(status_params[ID]->LABEL);
    tft.setCursor(status_params[ID]->VALUE_CURSOR_X, status_params[ID]->CURSOR_Y);
    tft.print(status_params[ID]->CURRENT_VALUE);
    return micros() - start;
}
/* END set_status_label_and_value() */

/* set_heading() */
unsigned long Display::set_heading(unsigned char ID)
{
    unsigned long start = micros();
    tft.setTextSize(1);
    tft.setTextColor(HX8357_WHITE);
    tft.setCursor(headings[ID]->LABEL_CURSOR_X, headings[ID]->CURSOR_Y);
    tft.print(headings[ID]->LABEL);
    return micros() - start;
}
/* END set_heading() */

/* update_numeric_output() */
unsigned long Display::update_numeric_output(unsigned char ID)
{
    unsigned long start = micros();

    tft.setTextSize(1);
    tft.setTextColor(HX8357_WHITE);
    tft.fillRect(numeric_params[ID]->VALUE_CURSOR_X, numeric_params[ID]->CURSOR_Y, BOX_WIDTH, BOX_HEIGHT, HX8357_BLACK);
    tft.setCursor(numeric_params[ID]->VALUE_CURSOR_X, numeric_params[ID]->CURSOR_Y);
    tft.print(numeric_params[ID]->CURRENT_VALUE);
    return micros() - start;
}
/* END update_numeric_output() */

/* update_status_output() */
unsigned long Display::update_status_output(unsigned char ID)
{
    unsigned long start = micros();
    tft.fillRect(status_params[ID]->VALUE_CURSOR_X, status_params[ID]->CURSOR_Y, BOX_WIDTH, BOX_HEIGHT, HX8357_BLACK);
    tft.setTextSize(1);
    tft.setTextColor(HX8357_WHITE);
    tft.setCursor(status_params[ID]->VALUE_CURSOR_X, status_params[ID]->CURSOR_Y);
    tft.print(status_params[ID]->CURRENT_VALUE);
    return micros() - start;
}
/* END update_status_output() */

/* poll_numeric_inputs() */
unsigned long Display::poll_numeric_inputs()
{
    unsigned long start = micros();
    for (int ID = 0; ID < NUMERIC_PARAM_COUNT; ID++)
    {
        if (numeric_params[ID]->CURRENT_VALUE != numeric_params[ID]->NEW_VALUE)
        { 
            numeric_params[ID]->CURRENT_VALUE = numeric_params[ID]->NEW_VALUE;
            update_numeric_output(ID);
            delay(1000);
        }
    }
    return micros() - start;
}
/* END poll_numeric_inputs() */

/* poll_status_inputs() */
unsigned long Display::poll_status_inputs()
{
    unsigned long start = micros();
    for (int ID = 0; ID < STATUS_PARAM_COUNT; ID++)
    {
        if (status_params[ID]->CURRENT_VALUE != status_params[ID]->NEW_VALUE)
        {
            status_params[ID]->CURRENT_VALUE = status_params[ID]->NEW_VALUE;
            update_status_output(ID);
            delay(1000);
        }
    }
    return micros() - start;
}
/* END poll_status_inputs() */

/* set_numeric_input_screen() */
unsigned long Display::set_numeric_input_screen()
{
    unsigned long start = micros();
    tft.fillScreen(HX8357_BLACK);
    tft.setTextSize(2);
    tft.setCursor(0, 15);
    tft.setTextColor(HX8357_WHITE);
    tft.print("Enter Amount:"); // text box for input amount begin at (180, 15)
    // horizontal lines
    tft.drawLine(0, 50, 480, 50,    HX8357_WHITE);
    tft.drawLine(0, 140, 480, 140,  HX8357_WHITE);
    tft.drawLine(0, 230, 480, 230,  HX8357_WHITE);
    // vertical lines for number pad
    tft.drawLine(96, 50, 96, 320,   HX8357_WHITE);
    tft.drawLine(192, 50, 192, 320, HX8357_WHITE);
    tft.drawLine(288, 50, 288, 320, HX8357_WHITE);
    tft.drawLine(384, 50, 384, 230, HX8357_WHITE);
    // draw numbers 1st row
    tft.setTextSize(2);
    int x_cursor = 42;
    int y2 = 6;

    tft.setCursor(42, 85);
    tft.print("1");
    tft.setCursor(138, 85);
    tft.print("2");
    tft.setCursor(234, 85);
    tft.print("3");
    tft.setCursor(330, 85);
    tft.print("4");
    tft.setCursor(426, 85);
    tft.print("5");
    // draw numbers 2nd row
    tft.setCursor(42, 175);
    tft.print("6");
    tft.setCursor(138, 175);
    tft.print("7");
    tft.setCursor(234, 175);
    tft.print("8");
    tft.setCursor(330, 175);
    tft.print("9");
    tft.setCursor(426, 175);
    tft.print("0");
    
    tft.setCursor(5, 265);
    tft.print("Go Back");
    tft.setCursor(115, 265);
    tft.print("Clear");
    tft.setTextSize(3);
    tft.setCursor(234, 255);
    tft.print(".");
    tft.setTextSize(2);
    tft.setCursor(360, 265);
    tft.print("Enter");
    return micros() - start;
}
/* END set_numeric_input_screen() */

/* get_numeric_user_input() */
float Display::get_numeric_user_input()
{
    unsigned long start = micros();
    unsigned long end = 0;

    const char STR_0[] = "12345";
    const char STR_1[] = "67890";
    const char STR_2[] = "bc.en";
    char *buttons[] = { STR_0, STR_1, STR_2 };
    
    tft.setTextSize(2);
    tft.setTextColor(HX8357_WHITE);

    while (button != "e")
    {
        TSPoint p = ts.getPoint();
        delay(300);
        if (p.z > 3)
        {
            const unsigned short COL_1_BOUND = 96;
            const unsigned short COL_2_BOUND = 192;
            const unsigned short COL_3_BOUND = 288;
            const unsigned short COL_4_BOUND = 384;
            const unsigned char  TEXT_BOX_BOUND = 50;
            const unsigned char  ROW_1_BOUND    = 140;
            const unsigned char  ROW_2_BOUND    = 230;

            user_press.x_scaled = (float)p.y * (480.0 / 1023.0);
            user_press.y_scaled = 320.0 - ((float)p.x * (320.0 / 1023.0));
            
            /*
            user_press.x_scaled = ((unsigned short)p.y * 15) >> 5;
            user_press.y_scaled = 320 - (((unsigned short)p.x * 5)  >> 4);
            */
            Serial.print("X = "); Serial.print(user_press.x_scaled);
            Serial.print("\tY = "); Serial.print(user_press.y_scaled);
            Serial.print("\tPressure = "); Serial.println(p.z);

            if      (user_press.x_scaled < (COL_1_BOUND - BOUND_BUFFER)) { col = 0; }
            else if (user_press.x_scaled > (COL_1_BOUND + BOUND_BUFFER) && user_press.x_scaled < (COL_2_BOUND - BOUND_BUFFER)) { col = 1; }
            else if (user_press.x_scaled > (COL_2_BOUND + BOUND_BUFFER) && user_press.x_scaled < (COL_3_BOUND - BOUND_BUFFER)) { col = 2; }
            else if (user_press.x_scaled > (COL_3_BOUND + BOUND_BUFFER) && user_press.x_scaled < (COL_4_BOUND - BOUND_BUFFER)) { col = 3; }
            else if (user_press.x_scaled > (COL_4_BOUND + BOUND_BUFFER) && user_press.x_scaled < X_MAX) { col = 4; }
            else    { col = 1000; }
            
            if      (user_press.y_scaled > (TEXT_BOX_BOUND + BOUND_BUFFER)  && user_press.y_scaled < (ROW_1_BOUND - BOUND_BUFFER)) { row = 0; }
            else if (user_press.y_scaled > (ROW_1_BOUND + BOUND_BUFFER) && user_press.y_scaled < (ROW_2_BOUND - BOUND_BUFFER))     { row = 1; }
            else if (user_press.y_scaled > (ROW_2_BOUND + BOUND_BUFFER) && user_press.y_scaled < Y_MAX)                            { row = 2; }
            else    { row = 1000; }
            
            user_press.x_scaled = 1000;
            user_press.y_scaled = 1000;
            
            button = buttons[row][col];
            
        }
        tft.setCursor(text_box_cursor_x, TEXT_BOX_CURSOR_Y);
        if (isDigit(button) || button == 'p')
        {
            tft.print(button);
            input_string = String(input_string) + String(button);
            text_box_cursor_x += 12;
            break;
        }
        else if (button == 'b')
            return 0.0;
        else if (button == 'c')
        {
            tft.fillRect(180, 15, 300, 15, HX8357_BLACK);
            text_box_cursor_x = STARTING_CURSOR_X;
            button = 'n';
            input_string = "";
            break;
        }
        else if (button == 'n')
            break;
        else
            input_value = input_string.toFloat();
    }
    end = micros() - start;
    return input_value;
}
/* END get_numeric_user_input() */
