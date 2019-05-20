#include <Arduino.h>
#include "smoothed_reader.h"

const uint8_t PIN_ANALOG = A2;
const uint8_t N_READINGS = 20;

Smoothed_Reader<uint8_t, N_READINGS> reader;

uint16_t waveform_to_amplitude(uint16_t raw_waveform)
{
  /*
    MAX4466 has VCC/2 base level and rail-to-rail output
    Input: 512 +/- 512
    Output: 0..512

     5V   |    -----
          |   /     \
    2.5V  | --       -
          |           \
    0V    |            ----
          ___________________
  */

  // 17 - 127 = -110 -> 110/255 = 0.43

  return abs((int16_t)raw_waveform - 512);
}

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
  pinMode(PIN_ANALOG, INPUT_PULLUP);

  // faster baud means less time spend off-code
  Serial.begin(115200);
  delay(1000);
}

void loop()
{
  uint16_t reading = analogRead(PIN_ANALOG);  // 512 +/- 512
  uint8_t scaled_amplitude = waveform_to_amplitude(reading)/2;

  reader.read(scaled_amplitude);

  if (every(10)){
    // Don't try to read it as text, use a plotter instead
    // Arduino IDE > Tools > Serial Plotter (check baud!)
    Serial.print(scaled_amplitude);
    Serial.print(",");
    Serial.print(reader.get_rolling_avg());

    Serial.println();
  }
}
