#include <Arduino.h>

const uint8_t PIN_ANALOG = A2;

bool every(uint16_t ms)
{
  static uint32_t next = 0;

  if (next < millis())
  {
    next = millis() + ms;
    return true;
  }
  return false;
}

void setup()
{
  pinMode(PIN_ANALOG, INPUT);

  // faster baud means less time spend off-code
  Serial.begin(115200);
  //Serial.begin(9600);
  delay(1000);
}

void loop()
{
  uint16_t reading = analogRead(PIN_ANALOG);
  uint8_t scaled_input = map(reading, 0, 1023, 0, 255);

  // Don't try to read it as text, use a plotter instead
  // Arduino IDE > Tools > Serial Plotter (check baud!)
  Serial.println(scaled_input);

  /*
  if (every(5 * 1000))
  {
    Serial.println(0);
    Serial.println(255);
    Serial.println(0);
  }*/
}
