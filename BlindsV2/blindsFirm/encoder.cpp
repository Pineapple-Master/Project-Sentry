#include "globals.h"

void setupEncoder(int CLK, int DT, int SW, int &lastCLK) {
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    pinMode(SW, INPUT_PULLUP);

    lastCLK = digitalRead(CLK);
}

void readEncoder(int CLK, int DT, int &lastCLK, int &counter) {
    int currentCLK = digitalRead(CLK);

    if (currentCLK != lastCLK && currentCLK == HIGH) {
        if (digitalRead(DT) != currentCLK) {
            counter++;
        } else {
            counter--;
        }

        Serial.println(counter);
    }

    lastCLK = currentCLK;
}