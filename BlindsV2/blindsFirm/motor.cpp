#include "globals.h"

int leftCurrentStep = 0;
int rightCurrentStep = 0;

int stepDelay = 2;
int lcurrentPosition = 0;
int rcurrentPosition = 0;

int leftmotorCounter = 0;
int rightmotorCounter = 0;

int syncedMotorCounter = 0;

#define STEPS_INCREMENT 64 // Number of steps for a full rotation (adjust as needed)

// Half-step sequence
int stepSequence[8][4] = {

    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 1},
    {1, 0, 0, 1}
};


void setupStepper(int pins[]) {

    pinMode(pins[0], OUTPUT);
    pinMode(pins[1], OUTPUT);
    pinMode(pins[2], OUTPUT);
    pinMode(pins[3], OUTPUT);
}

void stepMotor(int direction, int pins[], const char* motorName) {

    if (strcmp(motorName, "left") == 0) {
        leftCurrentStep += direction;
        if (leftCurrentStep > 7) {
            leftCurrentStep = 0;
        }
        if (leftCurrentStep < 0) {
            leftCurrentStep = 7;
        }
        
        digitalWrite(pins[0], stepSequence[leftCurrentStep][0]);
        digitalWrite(pins[1], stepSequence[leftCurrentStep][1]);
        digitalWrite(pins[2], stepSequence[leftCurrentStep][2]);
        digitalWrite(pins[3], stepSequence[leftCurrentStep][3]);

        delay(stepDelay);
    } else if (strcmp(motorName, "right") == 0) {
        rightCurrentStep += direction;
        if (rightCurrentStep > 7) {
            rightCurrentStep = 0;
        }
        if (rightCurrentStep < 0) {
            rightCurrentStep = 7;
        }
        digitalWrite(pins[0], stepSequence[rightCurrentStep][0]);
        digitalWrite(pins[1], stepSequence[rightCurrentStep][1]);
        digitalWrite(pins[2], stepSequence[rightCurrentStep][2]);
        digitalWrite(pins[3], stepSequence[rightCurrentStep][3]);

        delay(stepDelay);

    }


}

void rotateSteps( int direction, int pins[], const char* motorName) {

    for (int i = 0; i < STEPS_INCREMENT; i++) {
        stepMotor(direction, pins, motorName);
    }
}

void updateMotor(int targetPosition, int pins[], const char* motorName) {
    if (strcmp(motorName, "left") == 0) {
        if (leftmotorCounter < targetPosition) {
            rotateSteps(1, pins, motorName);
            leftmotorCounter = leftmotorCounter + STEPS_PER_ENCODER_TICK;
        }
        else if (leftmotorCounter > targetPosition) {
            rotateSteps(-1, pins, motorName);
            leftmotorCounter = leftmotorCounter - STEPS_PER_ENCODER_TICK;
        } else {
            stopMotor(pins);
        }
    }
    else if (strcmp(motorName, "right") == 0) {
        if (rightmotorCounter < targetPosition) {
            rotateSteps(1, pins, motorName);
            rightmotorCounter = rightmotorCounter + STEPS_PER_ENCODER_TICK;
        }
        else if (rightmotorCounter > targetPosition) {
            rotateSteps(-1, pins, motorName);
            rightmotorCounter = rightmotorCounter - STEPS_PER_ENCODER_TICK;
        } else {
            stopMotor(pins);
        }
    } 




}

void stopMotor(int pins[]) {

    digitalWrite(pins[0], LOW);
    digitalWrite(pins[1], LOW);
    digitalWrite(pins[2], LOW);
    digitalWrite(pins[3], LOW);

}