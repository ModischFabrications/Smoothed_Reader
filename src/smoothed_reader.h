#pragma once

#include <Arduino.h>



template <class INPUT_DATA_TYPE, uint8_t SAMPLE_SIZE>
class Smoothed_Reader
{
  private:
    INPUT_DATA_TYPE last_values[SAMPLE_SIZE]; // average over last readings

    // 255 samples (uint8_t) max is way too low with a target of 1s max.
    // A good ADC can reach over 100k Samples per second, most around 10k, a bad one 500.
    uint16_t next_writable_addr = 0;        

  public:
    Smoothed_Reader()
    {
        // init with zero
        for (uint16_t i = 0; i < SAMPLE_SIZE; i++)
        {
            last_values[i] = 0;
        }
    }

    void read(INPUT_DATA_TYPE value)
    {
        last_values[next_writable_addr] = value;

        // wrapping
        this->next_writable_addr++;
        if (this->next_writable_addr >= SAMPLE_SIZE)
        {
            this->next_writable_addr -= SAMPLE_SIZE;
        }
    }

    INPUT_DATA_TYPE get_rolling_avg()
    {
        INPUT_DATA_TYPE rolling_avg = 0;

        for (uint16_t i = 0; i < SAMPLE_SIZE; i++)
        {
            rolling_avg += (last_values[i] / SAMPLE_SIZE);
        }

        return rolling_avg;
    }

    // TODO: get avg of highest points
};
