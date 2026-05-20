
#include "globals.h"

bool calibration() {
    Serial.println("Starting calibration...");

    for (int i = 0; i < 10; i++) {
        digitalWrite(ledPin, HIGH);
        delay(300);

        digitalWrite(ledPin, LOW);
        delay(300);
    }

    return true;
}