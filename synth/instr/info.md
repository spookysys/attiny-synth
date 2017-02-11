# Instruments

In contrast to waveforms:
* more elaborate
* have state
* renders sound into buffers, 8 samples at a time

All the buffers are located in an array in the "globals" namespace. This way, buffers can be referred to with an 8-bit index instead of with a pointer.
