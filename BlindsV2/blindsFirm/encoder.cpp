#include "globals.h"

int volatile left_upper_bound = 10000; // Example bound for encoder counts, adjust as needed
int volatile right_upper_bound = 10000; // Example bound for encoder counts, adjust as needed

// Interrupt service routines
void IRAM_ATTR leftEncoderISR() {
    if (digitalRead(L_CLK) == HIGH) {
        if (digitalRead(L_DT) != digitalRead(L_CLK)) {
            const int lowerBound = (leftEncoderCalibratedState == 0) ? -9999 : 0;
            if (leftCounter > lowerBound) {
                leftCounter -= STEPS_PER_ENCODER_TICK;
            }
        } else {
            if (leftCounter < left_upper_bound) {
                leftCounter += STEPS_PER_ENCODER_TICK;
            }
        }
        
    }
}

void IRAM_ATTR rightEncoderISR() {
    if (digitalRead(R_CLK) == HIGH) {
        if (digitalRead(R_DT) != digitalRead(R_CLK)) {
            const int lowerBound = (rightEncoderCalibratedState == 0) ? -9999 : 0;
            if (rightCounter > lowerBound) {
                rightCounter -= STEPS_PER_ENCODER_TICK;
            }
        } else {
            if (rightCounter < right_upper_bound) {
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