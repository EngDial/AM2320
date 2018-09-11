#ifndef AM2320_H
#define AM2320_H

#include <Arduino.h>
#include <Wire.h>

#define AM2320_address (0xB8 >> 1) 

class AM2320
{
  private:
    TwoWire* communication;
  public:
    AM2320(TwoWire* com_wire);
    float cTemp;
    float Humidity;
    uint8_t State;
    uint8_t Read(void);
    uint8_t startConvert(void);
    uint8_t getData(void);
};

#endif
