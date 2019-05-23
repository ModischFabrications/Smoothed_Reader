#pragma once

// sampling rate varies between projects, skills and applications
// that's why this class takes a sample *size* instead of time
// keep in mind that this can really make your DATA usage explode

template <class INPUT_DATA_TYPE, uint16_t SAMPLE_SIZE>
class Smoothed_Reader
{
private:
    INPUT_DATA_TYPE last_values[SAMPLE_SIZE]; // average over last readings

    // previous 255 samples (uint8_t) max is way too low with a target of 1s max.
    // A good ADC can reach over 100k Samples per second, most around 10k, a bad one 500.
    uint16_t next_value_addr = 0;

    // these might be implemented in some library 
    static INPUT_DATA_TYPE array_avg(INPUT_DATA_TYPE array[], uint16_t size)
    {
        INPUT_DATA_TYPE avg = 0;

        for (uint16_t i = 0; i < size; i++)
        {
            avg += (array[i] / size);       // TODO: this might round down a lot
        }

        return avg;
    }

    static INPUT_DATA_TYPE array_max(INPUT_DATA_TYPE array[], uint16_t size)
    {
        INPUT_DATA_TYPE max = 0;

        for (uint16_t i = 0; i < size; i++)
        {
            INPUT_DATA_TYPE sample = array[i];
            if (sample > max) max = sample;
        }

        return max;
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
        last_values[next_value_addr] = value;

        // wrapping
        this->next_value_addr++;
        if (this->next_value_addr >= SAMPLE_SIZE)
        {
            this->next_value_addr -= SAMPLE_SIZE;
        }
    }

    /**
     * Get the average of all previous samples
     * */
    INPUT_DATA_TYPE get_rolling_avg()
    {
        return array_avg(this->last_values, SAMPLE_SIZE);
    }

    /**
     * Get the biggest value of the current sampling window
     * */
    INPUT_DATA_TYPE get_rolling_max()
    {
        // this could also find the last entry in max_history to save performance
        return array_max(this->last_values, SAMPLE_SIZE);
    }
};
