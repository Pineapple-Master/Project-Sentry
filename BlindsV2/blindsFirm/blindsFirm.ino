
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
int rightStepperPins[4] = {18, 19, 22, 23};

int calibrateButtonPin = 15;
int syncButtonPin = 12;
int automaticButtonPin = 21;

int ledPin = 2;
int ldrPin = 34;

int D_SCL = 13;
int D_SDA = 25;

//I2C



volatile int calibrationState = 0; // 0: Not calibrated, 1: Calibrating , 2: Calibrated
bool syncState = false; // 0: Not synced, 1: Syncing, 2: Synced
bool automaticState = false; // 0: Manual, 1: Automatic

int leftBlindsState = 0; // 0: Closed, 1: Open
int rightBlindsState = 0; // 0: Closed, 1: Open


// Encoder values
volatile int leftCounter = 0;
volatile int rightCounter = 0;
volatile int syncedCounter = 0;



int lastLeftCounter = 0;
int lastRightCounter = 0;



void instantControl(int swPin, const char* motorName, bool calibrationMode = false) {
    
      delay(50); // debounce
      Serial.println("Instant control triggered");
      noInterrupts();

      if (calibrationMode) {
          if (strcmp(motorName, "left") == 0) {
              leftCounter = 0; // Move to full closed position
              leftBlindsState = 0; // Update state to closed
          } else if (strcmp(motorName, "right") == 0) {
              rightCounter = 0; // Move to full closed position
              rightBlindsState = 0; // Update state to closed
          }
      } else {
        if ((strcmp(motorName, "left") == 0 ? leftBlindsState : rightBlindsState) == 0) {
            if (strcmp(motorName, "left") == 0) {
                leftCounter = left_upper_bound/2; // Move to full open position
            } else if (strcmp(motorName, "right") == 0) {
                rightCounter = right_upper_bound/2; // Move to full open position
                
            }

        } else if((strcmp(motorName, "left") == 0 ? leftBlindsState : rightBlindsState) == 1) {
            if (strcmp(motorName, "left") == 0) {
                leftCounter = 0; // Move to full closed position
                leftBlindsState = 0; // Update state to closed
            } else if (strcmp(motorName, "right") == 0) {
                rightCounter = 0; // Move to full closed position
                rightBlindsState = 0; // Update state to closed
            }
        }
      }
      interrupts();

}

void updateBlindsState(const char* motorName, long targetPosition) {
    if (strcmp(motorName, "left") == 0) {
        if (targetPosition >= (.2 * left_upper_bound) && targetPosition <= (.8 * left_upper_bound)) {
            leftBlindsState = 1; // Open
        } else {
            leftBlindsState = 0; // Closed
        }
    } else if (strcmp(motorName, "right") == 0) {
        if (targetPosition >= (.2 * right_upper_bound) && targetPosition <= (.8 * right_upper_bound)) {
            rightBlindsState = 1; // Open
        } else {
            rightBlindsState = 0; // Closed
        }
    }

}

int readSunlight() {
    int ldrValue = analogRead(ldrPin);
    return ldrValue;
}
void automaticControl() {
    int ldrValue = readSunlight();
    if (ldrValue > 1500) { // Threshold for darkness, adjust as needed
        // Open blinds
        if(leftBlindsState == 0) {
            leftCounter = left_upper_bound/2;
            leftBlindsState = 1; // Update state to open
            Serial.println("Automatic control: Opening left blinds");
        }
        if(rightBlindsState == 0) {
            rightCounter = right_upper_bound/2;
            rightBlindsState = 1; // Update state to open
            Serial.println("Automatic control: Opening right blinds");
        }
        
    } else {
        // Close blinds
        if(leftBlindsState == 1) {
            leftCounter = 0;
            leftBlindsState = 0; // Update state to closed
            Serial.println("Automatic control: Closing left blinds");
        }
        if(rightBlindsState == 1) {
            rightCounter = 0;
            rightBlindsState = 0; // Update state to closed
            Serial.println("Automatic control: Closing right blinds");
        }
    
        
    }
}



void setup() {
    Serial.begin(115200);

    pinMode(ledPin, OUTPUT);
    pinMode(calibrateButtonPin, INPUT_PULLUP);
    pinMode(syncButtonPin, INPUT_PULLUP);
    pinMode(automaticButtonPin, INPUT_PULLUP);
    setupEncoder(L_CLK, L_DT, L_SW);
    setupEncoder(R_CLK, R_DT, R_SW);
    
    // Attach interrupts to CLK pins on rising edge
    attachInterrupt(digitalPinToInterrupt(L_CLK), leftEncoderISR, RISING);
    attachInterrupt(digitalPinToInterrupt(R_CLK), rightEncoderISR, RISING);
    
    setupStepper(leftStepperPins);
    setupStepper(rightStepperPins); 
}

void loop() {
    //Calibration mode
    if (calibrationState == 0 || digitalRead(calibrateButtonPin) == LOW) {
      delay(50); // debounce
      calibrationState = 1;

      if (calibration()) {
          calibrationState = 2;
          Serial.println("Calibration successful!");
      } else {
          calibrationState = 0;
          Serial.println("Calibration failed. Please try again.");
      }
      
    }


  //Sync control mode
    if (digitalRead(syncButtonPin) == LOW) {
      delay(50); // debounce
      if (syncState == false) {
        syncState = true;
        Serial.println("Sync: True");
      } else {
        syncState = false;
        Serial.println("Sync: False");
      }
    }

    if (digitalRead(automaticButtonPin) == LOW) {
      while (digitalRead(automaticButtonPin) == LOW) {
        delay(10); // debounce
      }
      Serial.println("Automatic mode: False &&&&&&&&&&&&&&&&&&&");
      
      if (automaticState == false) {
        automaticState = true;
        Serial.println("Automatic mode: True -----------------");
      } else {
        automaticState = false;
        Serial.println("Automatic mode: False &&&&&&&&&&&&&&&&&&&");
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

    
    //Instant control mode
    if (digitalRead(L_SW) == LOW) {
      delay(50); // debounce
      instantControl(L_SW, "left");
    } 
    if (digitalRead(R_SW) == LOW) {
      delay(50); // debounce
      instantControl(R_SW, "right");
    }

    //Automatic control mode
    if (automaticState) {
      automaticControl();
    }

    
    updateBlindsState("left", leftTarget);
    updateBlindsState("right", rightTarget);
    
    updateMotor(leftTarget, leftStepperPins, "left");
    updateMotor(rightTarget, rightStepperPins, "right");

    readSunlight();

}
