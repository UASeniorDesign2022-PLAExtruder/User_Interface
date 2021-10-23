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

void Display::set_text(unsigned char S, unsigned short C) { tft.setTextSize(S); tft.setTextColor(C); }

/* set_output_screen() */
void Display::set_output_screen()
{
    tft.drawLine(230, 20, 230, 320, HX8357_WHITE); // draw vertical divider line x = 230, y = 20 -> y = Y_MAX
    set_text(2, HX8357_BLUE);
    tft.setCursor(title_heading.LABEL_CURSOR_X, title_heading.CURSOR_Y);
    
    tft.print(title_heading.LABEL);
    set_text(1, HX8357_WHITE);
    for (int ID = 1; ID < HEADING_COUNT; ID++)        { set_heading(ID); }
    // for (int ID = 0; ID < NUMERIC_PARAM_COUNT; ID++)  { set_numeric_label_and_value(ID); }
    for (int ID = 0; ID < NUMERIC_PARAM_COUNT; ID++)  { set_label_and_value(numeric_params, ID); }
    // for (int ID = 0; ID < STATUS_PARAM_COUNT; ID++)   { set_status_label_and_value(ID); }
    for (int ID = 0; ID < STATUS_PARAM_COUNT; ID++)   { set_label_and_value(status_params, ID); }
}
/* END set_output_screen() */

/* set_heading() */
void Display::set_heading(unsigned char ID)
{
    set_text(1, HX8357_WHITE);
    tft.setCursor(headings[ID]->LABEL_CURSOR_X, headings[ID]->CURSOR_Y);
    tft.print(headings[ID]->LABEL);
}

/* END set_heading() */

/* set_numeric_input_screen() */
void Display::set_numeric_input_screen()
{
    unsigned short x_cursor = 42;
    unsigned char row_1_item, row_2_item;
    unsigned short bottom_label_positions[] = { 5, 115, 235, 310 };
    char *bottom_labels[]                   = { "Go Back", "Clear", ".", "Enter" };
    
    tft.fillScreen(HX8357_BLACK);
    tft.setCursor(0, 15);
    set_text(2, HX8357_WHITE);
    tft.print("Enter Amount:");
    // horizontal lines
    for (unsigned short n = 0; n < 3; n++)
        tft.drawLine(0, UPPER_BOUND + (n * 90), 480, UPPER_BOUND + (n * 90), HX8357_WHITE);
    for (unsigned char n = 0; n < 4; n++)
        tft.drawLine(COL_WIDTH * (n + 1), UPPER_BOUND, COL_WIDTH * (n + 1), 320, HX8357_WHITE);
    for (unsigned char i = 0; i < 5; i++)
    {
        row_1_item = i + 1;
        row_2_item = i + 6;
        tft.setCursor(x_cursor, 85);
        tft.print(row_1_item);
        tft.setCursor(x_cursor, 175);
        if (row_2_item == 10) { row_2_item = 0; }
        tft.print(row_2_item);
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
float Display::get_numeric_user_input()
{
    unsigned long end = 0;
    unsigned char row = 0;
    unsigned char col = 0;
    char button = 'n';
    String input_string = "";
    float input_value = 0.0;
    const char STR_0[] = "12345";
    const char STR_1[] = "67890";
    const char STR_2[] = "bc.en";
    char *buttons[] = { STR_0, STR_1, STR_2 };
    unsigned short text_box_cursor_x = STARTING_CURSOR_X;
    
    set_text(2, HX8357_WHITE);

    while (button != "e")
    {
        TSPoint p = ts.getPoint();
        delay(300);
        if (p.z > 3)
        {
            user_press.x_scaled = (float)p.y * (480.0 / 1023.0);
            user_press.y_scaled = 320.0 - ((float)p.x * (320.0 / 1023.0));
            
            Serial.print("X = "); Serial.print(user_press.x_scaled);
            Serial.print("\tY = "); Serial.print(user_press.y_scaled);
            Serial.print("\tPressure = "); Serial.println(p.z);

            if      (user_press.x_scaled < (COL_WIDTH - BUFFER)) { col = 0; }
            else if (user_press.x_scaled > (COL_WIDTH + BUFFER) && user_press.x_scaled < ((COL_WIDTH * 2) - BUFFER)) { col = 1; }
            else if (user_press.x_scaled > ((COL_WIDTH * 2) + BUFFER) && user_press.x_scaled < ((COL_WIDTH * 3) - BUFFER)) { col = 2; }
            else if (user_press.x_scaled > ((COL_WIDTH * 3) + BUFFER) && user_press.x_scaled < ((COL_WIDTH * 4) - BUFFER)) { col = 3; }
            else if (user_press.x_scaled > ((COL_WIDTH * 4) + BUFFER) && user_press.x_scaled < 480) { col = 4; }
            else    { col = 1000; }
            
            if      (user_press.y_scaled > (UPPER_BOUND + BUFFER)  && user_press.y_scaled < (UPPER_BOUND + ROW_HEIGHT - BUFFER)) { row = 0; }
            else if (user_press.y_scaled > (UPPER_BOUND + ROW_HEIGHT + BUFFER) && user_press.y_scaled < (UPPER_BOUND + (ROW_HEIGHT * 2) - BUFFER)) { row = 1; }
            else if (user_press.y_scaled > (UPPER_BOUND + (ROW_HEIGHT * 2) + BUFFER) && user_press.y_scaled < 320) { row = 2; }
            else    { row = 1000; }
            
            user_press.x_scaled = 1000;
            user_press.y_scaled = 1000;
            button = buttons[row][col];
            
            if (isDigit(button) || button == '.')
            {
                text_box_cursor_x += 12;
                tft.setCursor(text_box_cursor_x, TEXT_BOX_CURSOR_Y);
                tft.print(button);
                input_string.concat(button);
                Serial.print("input_string: "); Serial.println(input_string.c_str());
            }
            else if (button == 'b')
                return 0.0;
            else if (button == 'c')
            {
                tft.fillRect(180, 15, 300, 15, HX8357_BLACK);
                text_box_cursor_x = STARTING_CURSOR_X;
                button = 'n';
                input_string = "";
            }
            
        }
    }
    input_value = input_string.toFloat();
    return input_value;
}
/* END get_numeric_user_input() */
