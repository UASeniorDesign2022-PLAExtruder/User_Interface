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

 #define NUMERIC_PARAM_COUNT         11
 #define STATUS_PARAM_COUNT          4
 #define HEADING_COUNT               4

 class Display
 {
    public:

        /* hardware interface objects */
        Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);
        TouchScreen ts = TouchScreen(XP, YP, XM, YM, 285);
        
        /* cursor constants */
        const unsigned char   X_MIN                     = 0;
        const unsigned char   Y_MIN                     = 20;
        const unsigned short  X_MAX                     = 480;
        const unsigned short  Y_MAX                     = 320;
        const unsigned char   LEFT_COL_LABEL_CURSOR_X   = 0;
        const unsigned char   LEFT_COL_VALUE_CURSOR_X   = 130;
        const unsigned char   DIVIDER_LINE_CURSOR_X     = 230;
        const unsigned char   RIGHT_COL_LABEL_CURSOR_X  = 235;
        const unsigned short  RIGHT_COL_VALUE_CURSOR_X  = 385;
        const unsigned char   ROW_0_CURSOR_Y            = 0;
        const unsigned char   ROW_1_CURSOR_Y            = 20;
        const unsigned char   ROW_2_CURSOR_Y            = 40;
        const unsigned char   ROW_3_CURSOR_Y            = 60;
        const unsigned char   ROW_4_CURSOR_Y            = 80;
        const unsigned char   ROW_5_CURSOR_Y            = 100;
        const unsigned char   ROW_6_CURSOR_Y            = 120;
        const unsigned char   ROW_7_CURSOR_Y            = 140;
        const unsigned char   ROW_8_CURSOR_Y            = 160;
        const unsigned char   ROW_9_CURSOR_Y            = 180;
        const unsigned char   ROW_10_CURSOR_Y           = 200;
        const unsigned char   ROW_11_CURSOR_Y           = 220;
        const unsigned char   ROW_12_CURSOR_Y           = 240;
        const unsigned char   BOX_WIDTH                 = 95;
        const unsigned char   BOX_HEIGHT                = 15;
        /* touch screen constants and variables */
        const unsigned short  BOUND_BUFFER              = 2;
        const unsigned short  TEXT_BOX_CURSOR_Y         = 15;  
        const unsigned short  STARTING_CURSOR_X         = 180;
        unsigned char         row                       = 0;
        unsigned char         col                       = 0;
        float x_scaled                                  = 0;
        float y_scaled                                  = 0;
        unsigned short text_box_cursor_x              = STARTING_CURSOR_X;
        
        char button =           'n';
        String  input_string =  "";
        float   input_value =   0.0;

        struct Numeric_Param
        {
          String LABEL;
          unsigned char ID;
          float CURRENT_VALUE;
          float NEW_VALUE;
          int LABEL_CURSOR_X;
          int VALUE_CURSOR_X;
          int CURSOR_Y;
        };
        
        struct Status_Param
        {
          String LABEL;
          unsigned char ID;
          String CURRENT_VALUE;
          String NEW_VALUE;
          int LABEL_CURSOR_X;
          int VALUE_CURSOR_X;
          int CURSOR_Y;
        };
        
        struct Heading
        {
          String LABEL;
          unsigned char ID;
          int LABEL_CURSOR_X;
          int CURSOR_Y;
        };

        struct X_Y
        {
            unsigned short x_scaled = 0;
            unsigned y_scaled = 0;
        };

        struct Status
        {
            const String NONE = "Unknown";
            const String READY   = "Ready";
            const String ON = "ON";
            const String OFF = "OFF";
            const String OPEN = "Open";
            const String CLOSED = "Closed";
            const String IN_PROGRESS = "In Progress";
            const String COMPLETE = "Complete";
        };

        /******************************************************************************************************************************************************/
        Status STATUS;
        X_Y user_press;
        Heading title_heading           = { "PLA/PET Extruder",  0, LEFT_COL_LABEL_CURSOR_X,  ROW_0_CURSOR_Y };
        Heading preparation_heading     = { "Preparation Stage", 1, LEFT_COL_LABEL_CURSOR_X,  ROW_1_CURSOR_Y };
        Heading extrusion_heading       = { "Extrusion Stage",   2, RIGHT_COL_LABEL_CURSOR_X, ROW_1_CURSOR_Y };
        Heading temps_heading           = { "Temperatures",      3, RIGHT_COL_LABEL_CURSOR_X, ROW_4_CURSOR_Y };
        
        Numeric_Param desired_yield     = { "Desired Yield: ",          0,  0.0, 5.5, LEFT_COL_LABEL_CURSOR_X,  LEFT_COL_VALUE_CURSOR_X,  ROW_3_CURSOR_Y };
        Numeric_Param ground_weight     = { "Ground Weight (kg): ",     1,  0.0, 5.5, LEFT_COL_LABEL_CURSOR_X,  LEFT_COL_VALUE_CURSOR_X,  ROW_4_CURSOR_Y };
        Numeric_Param zone_1_temp       = { "Zone 1: ",                 2,  0.0, 5.5, RIGHT_COL_LABEL_CURSOR_X, RIGHT_COL_VALUE_CURSOR_X, ROW_5_CURSOR_Y };
        Numeric_Param zone_2_temp       = { "Zone 2: ",                 3,  0.0, 5.5, RIGHT_COL_LABEL_CURSOR_X, RIGHT_COL_VALUE_CURSOR_X, ROW_6_CURSOR_Y };
        Numeric_Param zone_3_temp       = { "Zone 3: ",                 4,  0.0, 5.5, RIGHT_COL_LABEL_CURSOR_X, RIGHT_COL_VALUE_CURSOR_X, ROW_7_CURSOR_Y };
        Numeric_Param zone_4_temp       = { "Zone 4: ",                 5,  0.0, 5.5, RIGHT_COL_LABEL_CURSOR_X, RIGHT_COL_VALUE_CURSOR_X, ROW_8_CURSOR_Y };
        Numeric_Param zone_5_temp       = { "Zone 5: ",                 6,  0.0, 5.5, RIGHT_COL_LABEL_CURSOR_X, RIGHT_COL_VALUE_CURSOR_X, ROW_9_CURSOR_Y };
        Numeric_Param screw_speed       = { "Screw Speed [RPM]: ",      7,  0.0, 5.5, RIGHT_COL_LABEL_CURSOR_X, RIGHT_COL_VALUE_CURSOR_X, ROW_10_CURSOR_Y };
        Numeric_Param filament_diameter = { "Filament Diameter [mm]: ", 8,  0.0, 5.5, RIGHT_COL_LABEL_CURSOR_X, RIGHT_COL_VALUE_CURSOR_X, ROW_11_CURSOR_Y };
        Numeric_Param extruded_length   = { "Extruded Length [m]: ",    9,  0.0, 5.5, RIGHT_COL_LABEL_CURSOR_X, RIGHT_COL_VALUE_CURSOR_X, ROW_12_CURSOR_Y };
        Numeric_Param projected_yield   = { "Projected Yield [kg]: ",   10, 0.0, 5.5, RIGHT_COL_LABEL_CURSOR_X, RIGHT_COL_VALUE_CURSOR_X, ROW_3_CURSOR_Y };
        
        Status_Param hopper_lid_status  = { "Hopper Lid Status: ", 0,  STATUS.NONE, STATUS.READY, LEFT_COL_LABEL_CURSOR_X,  LEFT_COL_VALUE_CURSOR_X,  ROW_5_CURSOR_Y };
        Status_Param grinder_status     = { "Grinder On/Off: ",    1,  STATUS.NONE, STATUS.READY, LEFT_COL_LABEL_CURSOR_X,  LEFT_COL_VALUE_CURSOR_X,  ROW_6_CURSOR_Y };
        Status_Param preparation_status = { "Status: ",            2,  STATUS.NONE, STATUS.READY, LEFT_COL_LABEL_CURSOR_X,  LEFT_COL_VALUE_CURSOR_X,  ROW_2_CURSOR_Y };
        Status_Param extrusion_status   = { "Status: ",            3,  STATUS.NONE, STATUS.READY, RIGHT_COL_LABEL_CURSOR_X, RIGHT_COL_VALUE_CURSOR_X, ROW_2_CURSOR_Y };
        /******************************************************************************************************************************************************/
        Numeric_Param* numeric_params[NUMERIC_PARAM_COUNT]  = { &desired_yield, &ground_weight, &zone_1_temp,&zone_2_temp, &zone_3_temp, &zone_4_temp,
                                                                &zone_5_temp, &screw_speed, &filament_diameter, &extruded_length, &projected_yield };                                                  
        Status_Param* status_params[STATUS_PARAM_COUNT]     = { &hopper_lid_status, &grinder_status, &preparation_status, &extrusion_status };
        Heading* headings[HEADING_COUNT]                    = { &title_heading, &preparation_heading, &extrusion_heading, &temps_heading };
        /******************************************************************************************************************************************************/
        unsigned long set_output_screen();
        unsigned long set_numeric_label_and_value(unsigned char ID);
        unsigned long set_status_label_and_value(unsigned char ID);
        unsigned long set_heading(unsigned char ID);
        unsigned long update_numeric_output(unsigned char ID);
        unsigned long update_status_output(unsigned char ID);
        unsigned long poll_numeric_inputs();
        unsigned long poll_status_inputs();
        unsigned long set_numeric_input_screen();
        float get_numeric_user_input();
     
 };

 #endif
