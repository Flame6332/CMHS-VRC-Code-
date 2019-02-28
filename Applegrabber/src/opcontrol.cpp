#include "main.h"
#include "robotstate.h"
#include "robotconfig.h"
#include "controllibrary.h"

using namespace std;
using namespace pros;

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

void opcontrol() {
	int PARTYTIME = 420;

	const int dT = 9; // delta time, change in time, delay (ms)
	const float nonTurboDriveSpeed = 0.6;
	const float nonTurboRotationSpeed = 0.55;
 	float currentDriveSpeed = nonTurboDriveSpeed;
 	float currentRotationSpeed = nonTurboRotationSpeed;
	float vertical, horizontal;
	float averageDriveSpeed;
	float milisecondsRequiredToDisableAutoIntake = 700;
	float milisecondsLeftToDisableAutoIntake = milisecondsRequiredToDisableAutoIntake;
	bool isRamEnabled = false;

	while (420 == PARTYTIME) {

		//lcd::print(0, "%d %d %d", (lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		//                 (lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		//                 (lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		if ( ( abs(controlMaster.get_analog(ANALOG_LEFT_Y))
							+ abs(controlMaster.get_analog(ANALOG_RIGHT_X)) ) > 50) {
				cancelAutoAim(); // if you're trying to get out of it
		}

		if (!isAutoDriving) { // can't be controlling it if auto aiming
			if (controlMaster.get_digital(DIGITAL_L2)) {
				currentDriveSpeed = 1.0; // turbo
				currentRotationSpeed = 1.0; // turbo
			} else {
				currentDriveSpeed = nonTurboDriveSpeed;
				currentRotationSpeed = nonTurboRotationSpeed; }
				// STICK DRIVE CONTROLS
			vertical = controlMaster.get_analog(ANALOG_LEFT_Y) * currentDriveSpeed;
			horizontal = controlMaster.get_analog(ANALOG_RIGHT_X) * currentRotationSpeed;
				mFrontLeft.move( vertical + horizontal );
				mBackLeft.move( vertical + horizontal );
				mFrontRight.move( vertical - horizontal );
				mBackRight.move( vertical - horizontal );
		}

		if (controlMaster.get_digital(DIGITAL_L2) &&
						controlMaster.get_digital(DIGITAL_LEFT)) { calibrate(); }


		if (isAutoIntakeEnabled == true) {

			if (controlMaster.get_digital(DIGITAL_B)) {
				// if being held while enabled, start to count down
				milisecondsLeftToDisableAutoIntake -= dT;
				if (milisecondsLeftToDisableAutoIntake <= 0) {
					toggleAutoBallIntake();  //disable
					milisecondsLeftToDisableAutoIntake = milisecondsRequiredToDisableAutoIntake;
				}
			}
			else {
				milisecondsLeftToDisableAutoIntake = milisecondsRequiredToDisableAutoIntake;
			}

			if (controlMaster.get_digital_new_press(DIGITAL_DOWN)) { manualPurge(); }
		}
		else {
			// enable auto ball intake with the click of a button
			if (controlMaster.get_digital_new_press(DIGITAL_B)) { toggleAutoBallIntake(); }

			if (controlMaster.get_digital(DIGITAL_L1)) { turnOnIntake(); }
			else if (controlMaster.get_digital(DIGITAL_DOWN)) { reverseIntake(); }
			else { turnOffIntake(); }
			/*if (controlMaster.get_digital(DIGITAL_L1) || controlMaster.get_digital(DIGITAL_DOWN)) {
				toggleAutoBallIntake(); // disable it, since it must be enable due to previous if statement
			}*/
		}

		if (controlMaster.get_digital_new_press(DIGITAL_R2)) { flip(); }
		if (controlMaster.get_digital_new_press(DIGITAL_R1)) { disableFlippy(); }
		if (controlMaster.get_digital_new_press(DIGITAL_A)) { manualFire(); }
		if (controlMaster.get_digital_new_press(DIGITAL_Y)) { autoAim(); }

		if (controlMaster.get_digital_new_press(DIGITAL_X)) {
			 if (!isRamEnabled) {
				 isRamEnabled = true;
				 disableFlippy();
				 mRam.move_absolute(900, 200);
			 }
			 else {
				 isRamEnabled = false;
				 mRam.move_absolute(6, 200);
			 }
		 }

		//if (controlMaster.get_digital_new_press(DIGITAL_UP)) { ballCountUp(); }
		//if (controlMaster.get_digital_new_press(DIGITAL_DOWN)) { ballCountDown(); }

		/*if (!isFiring) {
			if (controlMaster.get_digital(DIGITAL_L1)) { mRam.move_velocity(200); }
			else if (mRam.get_position() > 7*180) { mRam.move_velocity(-100); }
			else if (mRam.get_position() < 7*50 && mRam.get_position() > 7*5) { mRam.move_velocity(-100); }
			else { mRam.set_brake_mode(MOTOR_BRAKE_BRAKE); stopMotor(mRam); }
		}*/



		delay(dT);

	}
}

// Motor RPM +-200

/*

	//int rumbleFrameCount;
	//bool shouldRumble = false;
	//bool isRumbling = false;



averageDriveSpeed = ( abs(mFrontLeft.get_actual_velocity()) +
	abs(mBackLeft.get_actual_velocity()) +
	abs(mFrontRight.get_actual_velocity()) +
	abs(mBackRight.get_actual_velocity()) ) / 4;

if (averageDriveSpeed > 200) {
	shouldRumble = true;
} else { shouldRumble = false; }
// Added in to accomodate for delay in control data
if (rumbleFrameCount >= 4) {
	if (shouldRumble && !isRumbling) {
		controlMaster.rumble("-"); isRumbling = true; }
	else {
		controlMaster.rumble(""); isRumbling = false; }
}
rumbleFrameCount++;*/
