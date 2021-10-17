/***************************************************
 * Extruder Controller TFT Display Prototype
 * extruder_display.ino
 * Wilson Woods
 * 10.17.2021
 ****************************************************/ 

#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"

// These are 'flexible' lines that can be changed
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8 // RST can be set to -1 if you tie it to Arduino's reset

/* data points to be updated */

#define GROUND_PLASTIC_WEIGHT       0
#define ZONE_1_TEMP                 1
#define ZONE_2_TEMP                 2
#define ZONE_3_TEMP                 3
#define ZONE_4_TEMP                 4
#define ZONE_5_TEMP                 5
#define SCREW_MOTOR_SPEED           6
#define FILAMENT_DIAMETER           7
#define EXTRUDED_LENGTH             8

#define HOPPER_LID_STATUS           0
#define GRINDER_ON_OFF              1
#define GRINDER_STAGE_STATUS        2
#define EXTRUSION_STAGE_STATUS      3

#define NUMERIC_PARAMETER_COUNT     9
#define FLAG_PARAMETER_COUNT        4

float currentValues[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
float updatedValues[] = {1.2, 125.7, 128.1, 133.2, 123.5, 134.7, 55.0, 7.76, 1.8};

int currentFlags[] = {2, 2, 2, 2};
int updatedFlags[] = {3, 3, 3, 3};

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

// SoftSPI - note that on some processors this might be *faster* than hardware SPI!
//Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, MOSI, SCK, TFT_RST, MISO);

/*
 * headings[0] = title
 * headings[1] = grinding stage
 * headings[2] = extrusion stage
 * headings[3] = temperatures
 * 
 * dataLabels[0] = hopper lid status
 * dataLabels[1] = grinder on/off
 * dataLabels[2] = ground plastic weight
 * datalabels[3] = grinder stage status
 * dataLabels[4] = zone 1
 * dataLabels[5] = zone 2
 * dataLabels[6] = zone 3
 * dataLabels[7] = zone 4
 * dataLabels[8] = zone 5
 * dataLabels[9] = screw speed
 * dataLabels[10] = filament diameter
 * dataLabels[11] = extruded length
 * dataLabels[12] = extrusion stage status
 */

char *headings[] = {"The Extraordinary PLA/PET Extruder", "Grinding Stage", "Extrusion Stage", "Temperatures"};
char *dataLabels[] = {"Hopper Lid: ", "Grinder: ", "Ground [kg]: ", "Status: ", "Zone 1: ", "Zone 2: ",
                      "Zone 3: ", "Zone 4: ", "Zone 5: ", "Screw Speed [RPM]: ", "Filament Diameter [mm]: ", "Extruded Length: ", "Status: "};

void setup()
{
  Serial.begin(9600);
  Serial.println("HX8357D Test!"); 

  tft.begin();

  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(HX8357_RDPOWMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(HX8357_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(HX8357_RDCOLMOD);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(HX8357_RDDIM);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(HX8357_RDDSDR);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
  Serial.println(F("Benchmark Time (microseconds)"));
  Serial.println(F("Done!"));

  tft.setRotation(1);
  tft.fillScreen(HX8357_BLACK);
  
  configureScreen();
}


void loop(void)
{
    // testText(title);
    pollNumericInputs(updatedValues, currentValues);
    pollFlagInputs(updatedFlags, currentFlags);
    delay(1000);
}


void configureScreen()
{
  
  // unsigned long start = micros();
  tft.drawLine(180, 20, 180, 320, HX8357_WHITE);
  char* initialStatus = "Unknown";
  
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_BLUE);
  tft.print(headings[0]);

  /* grinding stage output */
  tft.setTextSize(1);
  tft.setCursor(0, 20);
  tft.setTextColor(HX8357_WHITE);

  tft.print(headings[1]);           /* grinding stage heading */

  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.print(dataLabels[0]);         /* hopper lid status */
  tft.setCursor(120, 40);
  tft.print(initialStatus);

  tft.setCursor(0, 60);
  tft.setTextColor(HX8357_WHITE);
  tft.print(dataLabels[1]);         /* grinder on/off */
  tft.setCursor(120, 60);
  tft.print(initialStatus);

  tft.setCursor(0, 80);
  tft.setTextColor(HX8357_WHITE);
  tft.print(dataLabels[2]);         /* ground weight */
  tft.setCursor(120, 80);
  tft.print(currentValues[0]);

  tft.setCursor(0, 100);
  tft.setTextColor(HX8357_WHITE);
  tft.print(dataLabels[3]);         /* stage status */
  tft.setCursor(120, 100);
  tft.print(initialStatus);

/* extrusion stage output */
  tft.setCursor(200, 20);
  tft.setTextColor(HX8357_WHITE);
  tft.print(headings[2]);           /* extrusion stage heading */

  tft.setCursor(200, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.print(dataLabels[4]);         /* zone 1 */ 
  tft.setCursor(380, 40);
  tft.print(currentValues[1]);


  tft.setCursor(200, 60);
  tft.setTextColor(HX8357_WHITE);
  tft.print(dataLabels[5]);         /* zone 2 */ 
  tft.setCursor(380, 60);
  tft.print(currentValues[2]);


  tft.setCursor(200, 80);
  tft.setTextColor(HX8357_WHITE);
  tft.print(dataLabels[6]);         /* zone 3 */
  tft.setCursor(380, 80);
  tft.print(currentValues[3]);


  tft.setCursor(200, 100);
  tft.setTextColor(HX8357_WHITE);
  tft.print(dataLabels[7]);         /* zone 4 */
  tft.setCursor(380, 100);
  tft.print(currentValues[4]);


  tft.setCursor(200, 120);
  tft.setTextColor(HX8357_WHITE);
  tft.print(dataLabels[8]);         /* zone 5 */
  tft.setCursor(380, 120);
  tft.print(currentValues[5]);


  tft.setCursor(200, 140);
  tft.setTextColor(HX8357_WHITE);
  tft.print(dataLabels[9]);
  tft.setCursor(380, 140);
  tft.print(currentValues[6]);         /* screw speed */

  tft.setCursor(200, 160);
  tft.setTextColor(HX8357_WHITE);
  tft.print(dataLabels[10]);        /* filament diameter */
  tft.setCursor(380, 160);
  tft.print(currentValues[7]);

  tft.setCursor(200, 180);
  tft.setTextColor(HX8357_WHITE);
  tft.print(dataLabels[11]);
  tft.setCursor(380, 180);
  tft.print(currentValues[8]);        /* extruded length */

  tft.setCursor(200, 200);
  tft.setTextColor(HX8357_WHITE);
  tft.print(dataLabels[12]);        /* extrusion stage status */
  tft.setCursor(380, 200);
  tft.print(initialStatus);
  
  //return micros() - start;
}

void updateNumericOutput(short int parameterID, float parameterData)
{
  // unsigned long start = micros();

  int cursorX = 0;
  int cursorY = 0;
  int width = 0;
  int height = 15;
  short int hopperLidStatusFlag;
  short int grinderStatusFlag;
  short int grindingStageStatusFlag;
  short int extrusionStageStatusFlag;
  
  switch (parameterID)
  {
    case GROUND_PLASTIC_WEIGHT:
    {
      cursorX = 120;
      cursorY = 80;
      width = 60;
      break;
    }
    
    case ZONE_1_TEMP:
    {
      cursorX = 380;
      cursorY = 40;
      width = 100;
      break;
    }

    case ZONE_2_TEMP:
    {
      cursorX = 380;
      cursorY = 60;
      width = 100;
      break;
    }

    case ZONE_3_TEMP:
    {
      cursorX = 380;
      cursorY = 80;
      width = 100;
      break;
    }

    case ZONE_4_TEMP:
    {
      cursorX = 380;
      cursorY = 100;
      width = 100;
      break;
    }

    case ZONE_5_TEMP:
    {
      cursorX = 380;
      cursorY = 120;
      width = 100;
      break;
    }

    case SCREW_MOTOR_SPEED:
    {
      cursorX = 380;
      cursorY = 140;
      width = 100;
      break;
    }

    case FILAMENT_DIAMETER:
    {
      cursorX = 380;
      cursorY = 160;
      width = 100;
      break;
    }

    case EXTRUDED_LENGTH:
    {
      cursorX = 380;
      cursorY = 180;
      width = 100;
      break;
    }

    default:
    {
      break;
    }
  }

    tft.fillRect(cursorX, cursorY, width, height, HX8357_BLACK);
    tft.setCursor(cursorX, cursorY);
    tft.setTextSize(1);
    tft.setTextColor(HX8357_WHITE);
    tft.print(parameterData);
    
    // return micros() - start;
  }

void updateStatusOutput(short int parameterID, short int statusFlag)
{
  // unsigned long start = micros();

  int cursorX = 0;
  int cursorY = 0;
  int width = 0;
  int height = 15;
  
  switch (parameterID)
  {
    case HOPPER_LID_STATUS:
    {
      cursorX = 120;
      cursorY = 40;
      width = 60;
      break;
    }
    
    case GRINDER_ON_OFF:
    {
      cursorX = 120;
      cursorY = 60;
      width = 60;
      break;
    }

    case GRINDER_STAGE_STATUS:
    {
      cursorX = 120;
      cursorY = 100;
      width = 60;
      break;
    }

    case EXTRUSION_STAGE_STATUS:
    {
      cursorX = 380;
      cursorY = 200;
      width = 100;
      break;
    }
    
    default:
    {
      break;
    }
  }
    tft.fillRect(cursorX, cursorY, width, height, HX8357_BLACK);
    tft.setCursor(cursorX, cursorY);
    tft.setTextSize(1);
    tft.setTextColor(HX8357_WHITE);
    switch(statusFlag)
    {
      case 0:
      {
         if(parameterID == HOPPER_LID_STATUS)
         {
            tft.print("Open");
         }
         else
         {
            tft.print("OFF");
         }
         break;
      }
      case 1:
      {
        if(parameterID == HOPPER_LID_STATUS)
        {
          tft.print("Closed");
        }
        else if (parameterID == GRINDER_ON_OFF)
        {
          tft.print("ON");
        }
        else
        {
          tft.print("In progress");
        }
        break;
      }
      case 2:
      {
        tft.print("Unknown");
        break;
      }
      case 3:
      {
        tft.print("Ready");
        break;
      }
      case 4:
      {
        tft.print("Complete");
        break;
      }
      case 5:
      {
        tft.print("Update Fail");
        break;
      }
      default:
      {
        break;
      }
    }
  
  // return micros() - start;
}

/*
 * status flags
 * off or open                       0
 * on or closed or in progress       1
 * unknown                           2
 * ready                             3
 * complete                          4
 * failed to update                  5
 */

void pollNumericInputs(float updatedValues[], float currentValues[])
{
  // unsigned long start = micros();
  for (int parameterID = 0; parameterID < NUMERIC_PARAMETER_COUNT; parameterID++)
  {
    if (updatedValues[parameterID] != currentValues[parameterID])
    {
      updateNumericOutput(parameterID, updatedValues[parameterID]);
      currentValues[parameterID] = updatedValues[parameterID];
      delay(1000);
    }
  }
  
  // return micros() - start;
}

void pollFlagInputs(int updatedValues[], int currentValues[])
{
  // unsigned long start = micros();
  for (int parameterID = 0; parameterID < FLAG_PARAMETER_COUNT; parameterID++)
  {
    if (updatedValues[parameterID] != currentValues[parameterID])
    {
      updateStatusOutput(parameterID, updatedValues[parameterID]);
      currentValues[parameterID] = updatedValues[parameterID];
      delay(1000);
    }
  }
  
  // return micros() - start;
}
  

void testText()
{
  // unsigned long start = micros();
  tft.setTextSize(2);
  
  tft.setCursor(0, 0);
  tft.setTextColor(HX8357_BLUE);
  tft.print("Extruder");

  tft.setCursor(0, 20);
  tft.setTextColor(HX8357_WHITE);
  tft.print("Temperature");
  
  tft.setCursor(0, 40);
  tft.setTextColor(HX8357_WHITE);
  tft.print("Zone 1: ");
  tft.setTextColor(HX8357_RED);
  tft.println(123.456);

  tft.setCursor(0, 60);
  tft.setTextColor(HX8357_WHITE);
  tft.print("Zone 2: ");
  tft.setTextColor(HX8357_RED);
  tft.println(116.789);

  tft.setCursor(0, 80);
  tft.setTextColor(HX8357_WHITE);
  tft.print("Zone 3: ");
  tft.setTextColor(HX8357_RED);
  tft.println(978.789);

  tft.setCursor(0, 100);
  tft.setTextColor(HX8357_WHITE);
  tft.print("Zone 4: ");
  tft.setTextColor(HX8357_RED);
  tft.println(11.111);

  tft.setCursor(0, 120);
  tft.setTextColor(HX8357_WHITE);
  tft.print("Zone 5: ");
  tft.setTextColor(HX8357_RED);
  tft.println(365.247);

  tft.drawLine(0, 140, 480, 140, HX8357_WHITE);
  
  // return micros() - start;
}
