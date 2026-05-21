#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define STEPS_PER_ENCODER_TICK 32

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

extern Adafruit_SSD1306 display;

extern int D_SCL;
extern int D_SDA;   


// Left Encoder
extern int L_SW;
extern int L_DT;
extern int L_CLK;

// Right Encoder
extern int R_SW;
extern int R_DT;
extern int R_CLK;

extern int ledPin;

// Encoder counters
extern volatile int leftCounter;
extern volatile int rightCounter;

// Bounds
extern volatile int left_upper_bound;
extern volatile int right_upper_bound;

// Motor step counters
extern int leftMotorCounter;
extern int rightMotorCounter;

extern bool syncState;

extern volatile int calibrationState;
extern int calibrateButtonPin;

extern volatile int leftEncoderCalibratedState;
extern volatile int rightEncoderCalibratedState;




// Interrupt service routine prototypes
void leftEncoderISR();
void rightEncoderISR();

void setupEncoder(int CLK, int DT, int SW);

// Stepper Motor Pins
extern int leftStepperPins[];
extern int rightStepperPins[];

extern volatile int moveToken; // 0: No movement, 1: leftMotor moving, 2: rightMotor moving

// Stepper Functions
void setupStepper(int pins[]);

void updateMotor(int targetPosition, int pins[], const char* motorName);

void rotateSteps( int direction, int pins[], const char* motorName);

void stepMotor(int direction, int pins[], const char* motorName);

void stopMotor(int pins[]);


// firmware modes
bool calibration();

void instantControl(int swPin, const char* motorName, bool calibrationMode);

//Display functions

void setupDisplay();

void updateDisplay(int leftCounter, int rightCounter, int automaticState, int syncState, int calibrationState);

#endif