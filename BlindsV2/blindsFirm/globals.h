#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

// Left Encoder
extern int L_SW;
extern int L_DT;
extern int L_CLK;

// Right Encoder
extern int R_SW;
extern int R_DT;
extern int R_CLK;

// Encoder counters
extern int leftCounter;
extern int rightCounter;

// Encoder previous states
extern int leftLastCLK;
extern int rightLastCLK;

void setupEncoder(int CLK, int DT, int SW, int &lastCLK);
void readEncoder(int CLK, int DT, int &lastCLK, int &counter);

#endif