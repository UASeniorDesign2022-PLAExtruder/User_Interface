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

const unsigned char NUMERIC_PARAM_COUNT = 11;
const unsigned char STATUS_PARAM_COUNT = 4;
const unsigned char HEADING_COUNT = 4;

class Display
{
  public:
      /* hardware interface objects */
      Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);
      TouchScreen ts = TouchScreen(XP, YP, XM, YM, 285);
      
      /* cursor constants */
      const unsigned char BOX_WIDTH         = 95;
      const unsigned char BOX_HEIGHT        = 15;
      /* touch screen constants */
      const unsigned char BUFFER            = 2;
      const unsigned char TEXT_BOX_CURSOR_Y = 15;  
      const unsigned char STARTING_CURSOR_X = 180;
      const unsigned char COL_WIDTH         = 96;
      const unsigned char ROW_HEIGHT        = 90;
      const unsigned char UPPER_BOUND       = 50;


      struct Numeric_Param
      {
          String LABEL;
          unsigned char ID;
          float CURRENT_VALUE;
          float NEW_VALUE;
          unsigned short LABEL_CURSOR_X;
          unsigned short VALUE_CURSOR_X;
          unsigned char CURSOR_Y;
      };  
      struct Status_Param
      {
          String LABEL;
          unsigned char ID;
          String CURRENT_VALUE;
          String NEW_VALUE;
          unsigned short LABEL_CURSOR_X;
          unsigned short VALUE_CURSOR_X;
          unsigned char CURSOR_Y;
      };
      struct Heading
      {
          String LABEL;
          const unsigned char ID;
          unsigned short LABEL_CURSOR_X;
          unsigned char CURSOR_Y;
      };
      struct X_Y
      {
          float x_scaled = 0.0;
          float y_scaled = 0.0;
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
      Heading title_heading           = { "PLA/PET Extruder",  0, 0,  0  }; // row 0
      Heading preparation_heading     = { "Preparation Stage", 1, 0,  20 }; // row 1
      Heading extrusion_heading       = { "Extrusion Stage",   2, 235, 20 }; // row 1
      Heading temps_heading           = { "Temperatures",      3, 235, 80 }; // row 4
      
      Numeric_Param desired_yield     = { "Desired Yield: ",          0,  0.0, 5.5, 0,  130,  60  }; // row 3
      Numeric_Param ground_weight     = { "Ground Weight (kg): ",     1,  0.0, 5.5, 0,  130,  80  }; // row 4
      Numeric_Param zone_1_temp       = { "Zone 1: ",                 2,  0.0, 5.5, 235, 385, 100 }; // row 5
      Numeric_Param zone_2_temp       = { "Zone 2: ",                 3,  0.0, 5.5, 235, 385, 120 }; // row 6
      Numeric_Param zone_3_temp       = { "Zone 3: ",                 4,  0.0, 5.5, 235, 385, 140 }; // row 7
      Numeric_Param zone_4_temp       = { "Zone 4: ",                 5,  0.0, 5.5, 235, 385, 160 }; // row 8
      Numeric_Param zone_5_temp       = { "Zone 5: ",                 6,  0.0, 5.5, 235, 385, 180 }; // row 9
      Numeric_Param screw_speed       = { "Screw Speed [RPM]: ",      7,  0.0, 5.5, 235, 385, 200 }; // row 10
      Numeric_Param filament_diameter = { "Filament Diameter [mm]: ", 8,  0.0, 5.5, 235, 385, 220 }; // row 11
      Numeric_Param extruded_length   = { "Extruded Length [m]: ",    9,  0.0, 5.5, 235, 385, 240 }; // row 12
      Numeric_Param projected_yield   = { "Projected Yield [kg]: ",   10, 0.0, 5.5, 235, 385, 60  };  // row 3
      
      Status_Param hopper_lid_status  = { "Hopper Lid Status: ", 0, STATUS.NONE, STATUS.READY, 0,  130,  100 }; // row 5
      Status_Param grinder_status     = { "Grinder On/Off: ",    1, STATUS.NONE, STATUS.READY, 0,  130,  120 }; // row 6
      Status_Param preparation_status = { "Status: ",            2, STATUS.NONE, STATUS.READY, 0,  130,  40  }; // row 2
      Status_Param extrusion_status   = { "Status: ",            3, STATUS.NONE, STATUS.READY, 235, 385, 40  }; // row 2
      /******************************************************************************************************************************************************/
      Numeric_Param* numeric_params[NUMERIC_PARAM_COUNT]  = { &desired_yield, &ground_weight, &zone_1_temp,&zone_2_temp, &zone_3_temp, &zone_4_temp,
                                                              &zone_5_temp, &screw_speed, &filament_diameter, &extruded_length, &projected_yield };                                                  
      Status_Param* status_params[STATUS_PARAM_COUNT]     = { &hopper_lid_status, &grinder_status, &preparation_status, &extrusion_status };
      Heading* headings[HEADING_COUNT]                    = { &title_heading, &preparation_heading, &extrusion_heading, &temps_heading };
      /******************************************************************************************************************************************************/

      template <class T> void set_label_and_value(T Params_Array, unsigned char ID);
      template <class T> void update_output( T Params_Array, unsigned char ID );
      template <class T> void poll_inputs(T Params_Array, unsigned char SIZE);  
      void set_text(unsigned char S, unsigned short C);
      void set_output_screen();
      void set_heading(unsigned char ID);
      void set_numeric_input_screen();
      float get_numeric_user_input();   
   
};

template <class T> void Display::set_label_and_value(T Params_Array, unsigned char ID)
{
    set_text(1, HX8357_WHITE);
    tft.setCursor(Params_Array[ID]->LABEL_CURSOR_X, Params_Array[ID]->CURSOR_Y);
    tft.print(Params_Array[ID]->LABEL);
    tft.setCursor(Params_Array[ID]->VALUE_CURSOR_X, Params_Array[ID]->CURSOR_Y);
    tft.print(Params_Array[ID]->CURRENT_VALUE);
}

template <class T> void Display::update_output( T Params_Array, unsigned char ID )
{
    set_text(1, HX8357_WHITE);
    tft.fillRect(Params_Array[ID]->VALUE_CURSOR_X, Params_Array[ID]->CURSOR_Y, BOX_WIDTH, BOX_HEIGHT, HX8357_BLACK);
    tft.setCursor(Params_Array[ID]->VALUE_CURSOR_X, Params_Array[ID]->CURSOR_Y);
    tft.print(Params_Array[ID]->CURRENT_VALUE);
}

template <class T> void Display::poll_inputs(T Params_Array, unsigned char SIZE)
{
    for (int ID = 0; ID < SIZE; ID++)
    {
        if (Params_Array[ID]->CURRENT_VALUE != Params_Array[ID]->NEW_VALUE)
        { 
            Params_Array[ID]->CURRENT_VALUE = Params_Array[ID]->NEW_VALUE;
            update_output(Params_Array, ID);
            delay(1000);
        }
    }
}
      
#endif
