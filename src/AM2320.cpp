#include "AM2320.h"
#include <Wire.h>

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
AM2320::AM2320()
{
}

// ---=== Start convertion ===--
int AM2320::startConvert()
{
  State = 1;
  // Wake
  Wire.beginTransmission(AM2320_address);
  Wire.endTransmission();
  // Send request for read 4 bytes
  Wire.beginTransmission(AM2320_address);
  Wire.write(0x03);// запрос
  Wire.write(0x00); // с 0-го адреса
  Wire.write(0x04); // 4 байта
  if (Wire.endTransmission(1) == 0) State = 0;
  return State;	
}

int AM2320::getData()
{
byte buf[8];
uint8_t i; 
unsigned int crc_temp;
//------------------------------
  // clr buf
  for (i = 0; i < 8; i++) 
    buf[i] = 0x00; 
  State = 2;
  // Read data (8 byte = (2 Header)+(2 Hum)+(2 Temp)+(2 CRC16))
  Wire.requestFrom(AM2320_address, 8); 
  for (i = 0; i < 8; i++) 
    buf[i] = Wire.read();

  // Check CRC16
  crc_temp = buf[7] << 8;
  crc_temp += buf[6];
  if (crc_temp == CRC16(buf, 6)) 
  {
    State = 0;
    cTemp = ( ((buf[4] & 0x7F) << 8) + buf[5] ) / 10.0;
    cTemp = (buf[4] & 0x80) ? cTemp * (-1) : cTemp;
    Humidity = ( (buf[2] << 8) + buf[3] ) / 10.0;
  }
  return State;
}

// Read
int AM2320::Read()
{
  if (startConvert() != 0) return State;
  delay(2); // delay 2 ms
  return getData();
}
