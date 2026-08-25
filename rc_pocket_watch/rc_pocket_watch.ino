#include <BlockNot.h>

BlockNot serial_timer(50);

const int BIT_0 = 0;
const int BIT_1 = 1;
const int MARKER = 2;

const int data_pin = 3;

volatile unsigned long start_time_us = 0;
volatile unsigned long pulse_width_us = 0;
volatile unsigned long pulse_start_us = 0;
volatile unsigned long previous_pulse_start_us = 0;

volatile int received_bit = -1;
volatile int received_flag = 0;
volatile bool measuring_pulse = false;

void setup() {
  Serial.begin(9600);

  pinMode(data_pin, INPUT);

  attachInterrupt(
    digitalPinToInterrupt(data_pin),
    wwvb_interrupt,
    CHANGE
  );

  Serial.println("Finished Setup.");
}

void loop() {
  if (serial_timer.TRIGGERED) {

    if (received_flag) {

      noInterrupts();

      unsigned long width = pulse_width_us;
      unsigned long period =
        pulse_start_us - previous_pulse_start_us;

      int bit = received_bit;

      received_flag = 0;

      interrupts();

      Serial.print("Pulse = ");
      Serial.print(width / 1000.0);
      Serial.print(" ms");

      if (period > 0) {
        Serial.print("   Period = ");
        Serial.print(period / 1000.0);
        Serial.print(" ms");
      }

      Serial.print("   -> ");

      if (bit == BIT_0)
        Serial.println("0");

      else if (bit == BIT_1)
        Serial.println("1");

      else if (bit == MARKER)
        Serial.println("MARKER");
    }
  }
}

void wwvb_interrupt() {

  bool pin_state = digitalRead(data_pin);

  // Falling edge
  if (pin_state == LOW) {

    unsigned long now = micros();

    previous_pulse_start_us = pulse_start_us;
    pulse_start_us = now;

    start_time_us = now;
    measuring_pulse = true;
  }

  // Rising edge
  else {

    if (!measuring_pulse)
      return;

    measuring_pulse = false;

    unsigned long width =
      micros() - start_time_us;

    // Ignore pulses shorter than 150 ms
    if (width < 150000)
      return;

    // Ignore pulses longer than 950 ms
    if (width > 950000)
      return;

    pulse_width_us = width;

    if (width < 350000)
      received_bit = BIT_0;

    else if (width < 650000)
      received_bit = BIT_1;

    else
      received_bit = MARKER;

    received_flag = 1;
  }
}