
#include "globals.h"

volatile int leftEncoderCalibratedState = 0; //0 = not calibrated, 1 = lower limit calibrated, 2 = upper limit calibrated
volatile int rightEncoderCalibratedState = 0;

void flashLED() {
	for (int i = 0; i < 10; i++) {
		digitalWrite(ledPin, HIGH);
		delay(100);
		digitalWrite(ledPin, LOW);
		delay(100);
	}
}

void calibrateMotor(const char* motorName, int swPin) {
	
	
	while ((strcmp(motorName, "left") == 0)
       ? (leftEncoderCalibratedState < 2)
       : (rightEncoderCalibratedState < 2)) {

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
		



		if(digitalRead(swPin) == LOW || digitalRead(calibrateButtonPin) == LOW) {
			
			
			//Lower limit calibration
			if ((strcmp(motorName, "left") == 0)
				? (leftEncoderCalibratedState == 0)
				: (rightEncoderCalibratedState == 0)) {
				noInterrupts();
				//calibrate bounds
				if (strcmp(motorName, "left") == 0) {
					leftCounter = 0;
					leftMotorCounter = 0;
				} else if (strcmp(motorName, "right") == 0) {
					rightCounter = 0;
					rightMotorCounter = 0;
				}
				interrupts();

				//update calibration state
				if(strcmp(motorName, "left") == 0) {
					leftEncoderCalibratedState = 1; // Lower limit calibrated
				} else if(strcmp(motorName, "right") == 0) {
					rightEncoderCalibratedState = 1; // Lower limit calibrated
				}
				
				

				Serial.println("Lower limit calibrated.");



			// Upper limit calibration
			} else if ((strcmp(motorName, "left") == 0)
				? (leftEncoderCalibratedState == 1)
				: (rightEncoderCalibratedState == 1)) {

				if(strcmp(motorName, "left") == 0) {
					leftEncoderCalibratedState = 2; // Upper limit calibrated
					noInterrupts();
					left_upper_bound = leftCounter;
					interrupts();

				} else if(strcmp(motorName, "right") == 0) {
					rightEncoderCalibratedState = 2; // Upper limit calibrated
					noInterrupts();
					right_upper_bound = rightCounter;
					interrupts();
				}
				Serial.print("Upper limit calibrated: ");
				Serial.println(strcmp(motorName, "left") == 0 ? left_upper_bound : right_upper_bound);
			}

			//debounce
			while (digitalRead(swPin) == LOW || digitalRead(calibrateButtonPin) == LOW) {
				delay(10);
			}

		}
	}
}

bool calibration() {

	Serial.println("Starting calibration...");
	flashLED();

	bool oldSyncState = syncState;
	syncState = false; // Disable synchronization during calibration

	leftEncoderCalibratedState = 0;
	rightEncoderCalibratedState = 0;
	calibrateMotor("left", L_SW);
	calibrateMotor("right", R_SW);
	flashLED();
	syncState = oldSyncState; // Restore original synchronization state
	return true;
}





