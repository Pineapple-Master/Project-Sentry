#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

#define STEPS_PER_ENCODER_TICK 32

// Left Encoder
extern int L_SW;
extern int L_DT;
extern int L_CLK;

// Right Encoder
extern int R_SW;
extern int R_DT;
extern int R_CLK;

// Encoder counters
extern volatile int leftCounter;
extern volatile int rightCounter;

// Interrupt service routine prototypes
void leftEncoderISR();
void rightEncoderISR();

void setupEncoder(int CLK, int DT, int SW);

// Stepper Motor Pins
extern int stepperPins[4];


// Stepper Functions
void setupStepper(int pins[]);

void updateMotor(int targetPosition, int pins[], const char* motorName);

void rotateSteps( int direction, int pins[], const char* motorName);

void stepMotor(int direction, int pins[], const char* motorName);

void stopMotor(int pins[]);

#endif