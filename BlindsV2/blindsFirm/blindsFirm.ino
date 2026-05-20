
#include "globals.h"


//Left Encoder
int L_SW = 14;
int L_DT = 27;
int L_CLK = 26;

//Right Encoder
int R_SW = 33;
int R_DT = 32;
int R_CLK = 35;

//Stepper Left
int leftStepperPins[4] = {4, 16, 17, 5};
int rightStepperPins[4] = {19, 21, 22, 23};



// Encoder values
volatile int leftCounter = 0;
volatile int rightCounter = 0;



int lastLeftCounter = 0;
int lastRightCounter = 0;

void setup() {
    Serial.begin(115200);

    setupEncoder(L_CLK, L_DT, L_SW);
    setupEncoder(R_CLK, R_DT, R_SW);
    
    // Attach interrupts to CLK pins on rising edge
    attachInterrupt(digitalPinToInterrupt(L_CLK), leftEncoderISR, RISING);
    attachInterrupt(digitalPinToInterrupt(R_CLK), rightEncoderISR, RISING);
    
    setupStepper(leftStepperPins);
    setupStepper(rightStepperPins); 
}

void loop() {
    long leftTarget, rightTarget;

    noInterrupts();
    leftTarget = leftCounter;
    rightTarget = rightCounter;
    interrupts();

    updateMotor(leftTarget, leftStepperPins, "left");
    updateMotor(rightTarget, rightStepperPins, "right");
    

}