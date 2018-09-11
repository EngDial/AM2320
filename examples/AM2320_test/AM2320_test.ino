#include <AM2320.h>

AM2320 th(&Wire);

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  Serial.println("Chip = AM2320");
  switch(th.Read()) {
    case 2:
      Serial.println("  CRC failed");
      break;
    case 1:
      Serial.println("  Sensor offline");
      break;
    case 0:
      Serial.print("  Humidity = ");
      Serial.print(th.Humidity);
      Serial.println("%");
      Serial.print("  Temperature = ");
      Serial.print(th.cTemp);
      Serial.println("*C");
      Serial.println();
      break;
  }
  delay(2000);
}
