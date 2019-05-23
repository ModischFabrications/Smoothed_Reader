# Smoothed Reader
This Reader can be used to smooth out analog values with averages and more.
Look at main.cpp to see how this class is used. 
Be aware that readers can be cascaded.

This is also a convenient resource on how to convert microphone readings from 
a sensor board like the MAX4466 into an usable amplitude for 1 bar equalizers. 

## Example plot
![Annotation 2019-05-23 204317](https://user-images.githubusercontent.com/25404728/58278131-95509580-7d9b-11e9-9e5c-06e83204db78.png)

It can be seen that the raw input carries a lot of noise. The first averaging pass removes most of it, leaving only a small signal. The second pass uses it's maximums to widen the signal and convert it into a slower, more usable plot. The last pass remaps these values into the full range again.

This is another spin-off from https://github.com/ModischFabrications/UV_Meter_Code
