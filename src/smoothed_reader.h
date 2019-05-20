#pragma once

// sampling rate varies between projects, skills and applications
// that's why this class takes a sample *size* instead of time

template <class INPUT_DATA_TYPE, uint8_t SAMPLE_SIZE>
class Smoothed_Reader
{
  private:
    INPUT_DATA_TYPE last_values[SAMPLE_SIZE]; // average over last readings

    // previous 255 samples (uint8_t) max is way too low with a target of 1s max.
    // A good ADC can reach over 100k Samples per second, most around 10k, a bad one 500.
    uint16_t next_writable_addr = 0;        

    // array must be passed in to prevent it from being lost from scope
    void get_n_biggest(uint8_t n, INPUT_DATA_TYPE* out_biggest)
    {   
        // zero out array before working with it
        for (uint8_t i = 0; i < n; i++)
        {
            out_biggest[i] = 0;
        }

        for (uint16_t i_sample = 0; i_sample < SAMPLE_SIZE; i_sample++)
        {
            INPUT_DATA_TYPE sample = last_values[i_sample];

            // FIMXE: this is probably bullshit, think about it again
            for (uint8_t i_biggest = 0; i_biggest < n; i_biggest++)
            {
                if (sample > out_biggest[i_biggest]) 
                {
                    out_biggest[i_biggest] = sample;
                    break;
                }
            }
        }
    }

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

    /**
     * Get the average of all previous samples
     * */
    INPUT_DATA_TYPE get_rolling_avg()
    {
        INPUT_DATA_TYPE rolling_avg = 0;

        for (uint16_t i = 0; i < SAMPLE_SIZE; i++)
        {
            rolling_avg += (last_values[i] / SAMPLE_SIZE);
        }

        return rolling_avg;
    }
    /**
     * Get a peak-resistant max
     * */
    INPUT_DATA_TYPE get_rounded_max(uint8_t n_samples)
    {
        INPUT_DATA_TYPE max_avg = 0;

        // size is unknown, large buffers are safer than malloc()
        INPUT_DATA_TYPE biggest[UINT8_MAX]; 
        get_n_biggest(n_samples, biggest);

        for (uint8_t i = 0; i < n_samples; i++)
        {
            max_avg += biggest[i] / n_samples;
        }

        return max_avg;
    }

    // IDEA: It might be cool to have max values from the last n steps, this makes peaks wider
};
