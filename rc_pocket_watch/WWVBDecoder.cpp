#include "WWVBDecoder.h"

// Static state variables for the decoder
static int _data_pin;
static volatile unsigned long pulse_width_us = 0;
static volatile unsigned long pulse_start_us = 0;
static volatile unsigned long previous_pulse_start_us = 0;
static volatile unsigned long start_time_us = 0;
static volatile int received_bit = -1;
static volatile int received_flag = 0;
static volatile bool measuring_pulse = false;

// Interrupt Service Routine to measure pulse widths
static void handleInterrupt() {
    bool pin_state = digitalRead(_data_pin);
    unsigned long now = micros();

    if (pin_state == LOW) { // Falling edge: pulse started
        previous_pulse_start_us = pulse_start_us;
        pulse_start_us = now;
        start_time_us = now;
        measuring_pulse = true;
    } else { // Rising edge: pulse ended
        if (!measuring_pulse) return;
        measuring_pulse = false;

        unsigned long width = micros() - start_time_us;
        
        // Ignore noise pulses outside of expected WWVB duration (150ms-950ms)
        if (width < 150000 || width > 950000) return;

        pulse_width_us = width;

        // Decode pulse width to bit type
        if (width < 350000) received_bit = BIT_0;        // ~200ms
        else if (width < 650000) received_bit = BIT_1;   // ~500ms
        else received_bit = MARKER;                      // ~800ms
        
        received_flag = 1; // Signal that new data is ready
    }
}

void WWVB_init(int data_pin) {
    _data_pin = data_pin;
    pinMode(_data_pin, INPUT);
    // Attach interrupt to detect both falling and rising edges
    attachInterrupt(digitalPinToInterrupt(_data_pin), handleInterrupt, CHANGE);
}

bool WWVB_hasNewData() {
    return received_flag;
}

void WWVB_getPulseData(unsigned long& width, unsigned long& period, int& bit) {
    // Disable interrupts briefly to ensure atomic access to shared variables
    noInterrupts();
    width = pulse_width_us;
    period = pulse_start_us - previous_pulse_start_us;
    bit = received_bit;
    received_flag = 0; // Clear flag after reading
    interrupts();
}
