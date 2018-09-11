#include <AM2320.h>

AM2320 th(&Wire);

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  Serial.println(F("Chip = AM2320"));
  switch(th.Read()) {
    case 2:
      Serial.println(F("  CRC failed"));
      break;
    case 1:
      Serial.println(F("  Sensor offline"));
      break;
    case 0:
      Serial.print(F("  Humidity = "));
      Serial.print(th.Humidity);
      Serial.println(F("%"));
      Serial.print(F("  Temperature = "));
      Serial.print(th.cTemp);
      Serial.println(F("*C"));
      Serial.println();
      break;
  }
  delay(2000);
}
