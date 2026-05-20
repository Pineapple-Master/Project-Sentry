
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

int calibrateButtonPin = 18;
int syncButtonPin = 15;

int ledPin = 2;

int calibrationState = 0; // 0: Not calibrated, 1: Calibrating, 2: Calibrated
bool syncState = false; // 0: Not synced, 1: Syncing, 2: Synced





// Encoder values
volatile int leftCounter = 0;
volatile int rightCounter = 0;
volatile int syncedCounter = 0;



int lastLeftCounter = 0;
int lastRightCounter = 0;

void setup() {
    Serial.begin(115200);

    pinMode(ledPin, OUTPUT);
    pinMode(calibrateButtonPin, INPUT_PULLUP);
    pinMode(syncButtonPin, INPUT_PULLUP);

    setupEncoder(L_CLK, L_DT, L_SW);
    setupEncoder(R_CLK, R_DT, R_SW);
    
    // Attach interrupts to CLK pins on rising edge
    attachInterrupt(digitalPinToInterrupt(L_CLK), leftEncoderISR, RISING);
    attachInterrupt(digitalPinToInterrupt(R_CLK), rightEncoderISR, RISING);
    
    setupStepper(leftStepperPins);
    setupStepper(rightStepperPins); 
}

void loop() {
    if (calibrationState == 0 || digitalRead(calibrateButtonPin) == LOW) {
      delay(50); // debounce

      if (digitalRead(calibrateButtonPin) == LOW) {
        calibrationState = 1;

        if (calibration()) {
            calibrationState = 2;
            Serial.println("Calibration successful!");
        } else {
            calibrationState = 0;
            Serial.println("Calibration failed. Please try again.");
        }
      }
    }

    if (digitalRead(syncButtonPin) == LOW) {
      delay(50); // debounce
      if (syncState == false) {
        syncState = true;
      } else {
        syncState = false;
      }
    }

    
    


    long leftTarget, rightTarget;

 
    


    noInterrupts();
    if (syncState) {
      rightCounter = leftCounter;
    }
    leftTarget = leftCounter;
    rightTarget = rightCounter;
    interrupts();
    

    updateMotor(leftTarget, leftStepperPins, "left");
    updateMotor(rightTarget, rightStepperPins, "right");

    

}