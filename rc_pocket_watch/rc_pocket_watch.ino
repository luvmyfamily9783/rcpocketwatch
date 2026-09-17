// Main Sketch for WWVB Clock
#include <BlockNot.h>
#include "WWVBDecoder.h"

const int DATA_PIN = 3;
const int EN_PIN = 4;
BlockNot serial_timer(50); // Timer to throttle serial output

void setup() {
  Serial.begin(9600);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, HIGH);
  WWVB_init(DATA_PIN);
  Serial.println("Finished Setup.");
}

void loop() {
  static int bits_received = 0;

  // Process only if the timer is triggered and new data is ready
  if (serial_timer.TRIGGERED && WWVB_hasNewData()) {
    unsigned long width, period;
    int bit;
    WWVB_getPulseData(width, period, bit);

    // Concise output: print just the bit type
    if (bit == BIT_0) Serial.print("0 ");
    else if (bit == BIT_1) Serial.print("1 ");
    else if (bit == MARKER) Serial.print("M ");

    bits_received++;
    
    // Add periodic summary output
    if (bits_received % 10 == 0) {
      Serial.print(" | Bits received: ");
      Serial.println(bits_received);
    }
  }
}
