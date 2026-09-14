#ifndef WWVB_DECODER_H
#define WWVB_DECODER_H

#include <Arduino.h>

// WWVB Signal Bit definitions
const int BIT_0 = 0;
const int BIT_1 = 1;
const int MARKER = 2;

// Initializes the WWVB decoder on the specified pin
void WWVB_init(int data_pin);

// Checks if new pulse data is available
bool WWVB_hasNewData();

// Retrieves the latest pulse data (width, period, and decoded bit)
void WWVB_getPulseData(unsigned long& width, unsigned long& period, int& bit);

#endif
