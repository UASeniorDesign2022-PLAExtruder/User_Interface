// Author : Sandhan Sarma <sandhan.sarma26@gmail.com>
// Date : 2nd December 2018

#include <Wire.h>

float voltage = 0.0F;

union floatToBytes
{
    char buffer[4];
    float voltageReading;
};

union floatToBytes converter;
unsigned char index = 0;

byte dataIDs[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
byte status_param_code = 2;
void setup() {
  // put your setup code here, to run once:
  Wire.setClock(400000);
  Wire.begin(); // any 7-bit Slave address
  
  // Wire.onRequest(requestEvent); //Interrupt for handling incomming requests
  
}

void loop() {

  converter.voltageReading = 24.456;
  Wire.beginTransmission(0x14);
  Wire.write(dataIDs[index]);
  if (index > 3)
      Wire.write(converter.buffer, 4);
  else
      Wire.write(&status_param_code, 1);
  Wire.endTransmission();
  delay(500);
  index++;
  if (index == 12) { index = 0; }
}

// void requestEvent() {}
