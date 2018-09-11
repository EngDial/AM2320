#include "AM2320.h"

// ---=== Calc CRC16 ===---
uint16_t CRC16(uint8_t *ptr, uint8_t length) 
{ 
uint16_t crc = 0xFFFF; 
uint8_t i; 
//------------------------------
  while(length--) 
  {
    crc ^= *ptr++; 
    for (i = 0; i < 8; i++) 
      if ((crc & 0x01) != 0)
      {
        crc >>= 1; 
        crc ^= 0xA001; 
      }
      else 
        crc >>= 1; 
  } 
  return crc; 
} 

// ---=== Constructor ===---
AM2320::AM2320(TwoWire* com_wire)
{
  communication = com_wire;
}

// ---=== Start convertion ===--
uint8_t AM2320::startConvert()
{
  State = 1;
  // Wake
  communication->beginTransmission(AM2320_address);
  communication->endTransmission();
  // Send request for read 4 bytes
  communication->beginTransmission(AM2320_address);
  communication->write(0x03); // request
  communication->write(0x00); // from 0 address
  communication->write(0x04); // 4 bytes for read
  State = communication->endTransmission(1);
  return State;	
}

uint8_t AM2320::getData()
{
uint8_t buf[8];
uint8_t i; 
uint16_t crc_temp;
//------------------------------
  State = 2;
  for (i = 0; i < 8; i++) buf[i] = 0x00; 
  // Read data (8 byte = (2 Header)+(2 Hum)+(2 Temp)+(2 CRC16))
  communication->requestFrom(AM2320_address, 8); 
  for (i = 0; i < 8; i++) buf[i] = communication->read();
  // Check CRC16
  crc_temp = buf[7] << 8;
  crc_temp += buf[6];
  if (crc_temp == CRC16(buf, 6)) 
  {
    State = 0;
    cTemp = (((buf[4] & 0x7F) << 8) + buf[5]) / 10.0;
    cTemp = (buf[4] & 0x80) ? -cTemp : cTemp;
    Humidity = ((buf[2] << 8) + buf[3]) / 10.0;
  }
  return State;
}

// Read
uint8_t AM2320::Read()
{
  if (startConvert() != 0) return State;
  delay(2); // delay 2 ms
  return getData();
}
