#include "globals.h"

int upper_bound = 10000; // Example bound for encoder counts, adjust as needed

// Interrupt service routines
void IRAM_ATTR leftEncoderISR() {
    if (digitalRead(L_CLK) == HIGH) {
        if (digitalRead(L_DT) != digitalRead(L_CLK)) {
            if (leftCounter > 0) {
                leftCounter -= STEPS_PER_ENCODER_TICK;
            }
        } else {
            if (leftCounter < upper_bound) {
                leftCounter += STEPS_PER_ENCODER_TICK;
            }
        }
        
    }
}

void IRAM_ATTR rightEncoderISR() {
    if (digitalRead(R_CLK) == HIGH) {
        if (digitalRead(R_DT) != digitalRead(R_CLK)) {
            if (rightCounter > 0) {
                rightCounter -= STEPS_PER_ENCODER_TICK;
            }
        } else {
            if (rightCounter < upper_bound) {
                rightCounter += STEPS_PER_ENCODER_TICK;
            }
        }
        
    }
}

void setupEncoder(int CLK, int DT, int SW) {
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    pinMode(SW, INPUT_PULLUP);
}