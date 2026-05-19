#include "globals.h"

//Left Encoder
int L_SW = 15;
int L_DT = 2;
int L_CLK = 4;

//Right Encoder
int R_SW = 18;
int R_DT = 19;
int R_CLK = 21;

//Stepper Left
int L_IN1 = 13;
int L_IN2 = 12;
int L_IN3 = 14;
int L_IN4 = 27;



// Encoder values
int leftCounter = 0;
int rightCounter = 0;

// Previous CLK states
int leftLastCLK;
int rightLastCLK;

void setup() {
    Serial.begin(115200);

    setupEncoder(L_CLK, L_DT, L_SW, leftLastCLK);
    setupEncoder(R_CLK, R_DT, R_SW, rightLastCLK);
}

void loop() {
    readEncoder(L_CLK, L_DT, leftLastCLK, leftCounter);
    readEncoder(R_CLK, R_DT, rightLastCLK, rightCounter);
}