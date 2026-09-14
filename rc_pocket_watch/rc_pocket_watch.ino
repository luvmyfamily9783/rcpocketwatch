// Main Sketch for WWVB Clock
#include <BlockNot.h>
#include "WWVBDecoder.h"

const int DATA_PIN = 3;
BlockNot serial_timer(50); // Timer to throttle serial output

void setup() {
  Serial.begin(9600);
  WWVB_init(DATA_PIN);
  Serial.println("Finished Setup.");
}

void loop() {
  // Process only if the timer is triggered and new data is ready
  if (serial_timer.TRIGGERED && WWVB_hasNewData()) {
    unsigned long width, period;
    int bit;
    WWVB_getPulseData(width, period, bit);

    // Print pulse information to serial
    Serial.print("Pulse = "); Serial.print(width / 1000.0); Serial.print(" ms");
    if (period > 0) {
      Serial.print("   Period = "); Serial.print(period / 1000.0); Serial.print(" ms");
    }
    Serial.print("   -> ");
    
    // Print decoded bit type
    if (bit == BIT_0) Serial.println("0");
    else if (bit == BIT_1) Serial.println("1");
    else if (bit == MARKER) Serial.println("MARKER");
  }
}

    Serial.print("Pulse = "); Serial.print(width / 1000.0); Serial.print(" ms");
    if (period > 0) {
      Serial.print("   Period = "); Serial.print(period / 1000.0); Serial.print(" ms");
    }
    Serial.print("   -> ");
    
    if (bit == BIT_0) Serial.println("0");
    else if (bit == BIT_1) Serial.println("1");
    else if (bit == MARKER) Serial.println("MARKER");
  }
}
