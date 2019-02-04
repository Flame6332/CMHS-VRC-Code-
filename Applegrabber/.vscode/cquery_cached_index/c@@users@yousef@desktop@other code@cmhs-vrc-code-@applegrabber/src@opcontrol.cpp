#include "main.h"
#include "robotstate.h"
#include "robotconfig.h"

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

void calibrate();
void flip();

void opcontrol() {
	int PARTYTIME = 420;

	float nonTurboSpeed = 0.3;
 	float currentSpeed = nonTurboSpeed;
	int vertical, horizontal;
	float averageDriveSpeed;

	//int rumbleFrameCount;
	//bool shouldRumble = false;
	//bool isRumbling = false;


	controlPartner.print(0, 4, "poopy");

	while (420 == PARTYTIME) {

		lcd::print(0, "%d %d %d", (lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);



		if (controlMaster.get_digital(DIGITAL_L2)) {
			currentSpeed = 1.0; // turbo
		} else { currentSpeed = nonTurboSpeed; }
			// STICK DRIVE CONTROLS
		vertical = controlMaster.get_analog(ANALOG_LEFT_Y);
		horizontal = controlMaster.get_analog(ANALOG_RIGHT_X);
			mFrontLeft.move( (vertical+horizontal)*currentSpeed );
			mBackLeft.move( (vertical+horizontal)*currentSpeed );
			mFrontRight.move( (vertical-horizontal)*currentSpeed );
			mBackRight.move( (vertical-horizontal)*currentSpeed );


		if (controlMaster.get_digital(DIGITAL_L2) &&
				controlMaster.get_digital(DIGITAL_LEFT)) {
					calibrate();
		}


		if (controlMaster.get_digital_new_press(DIGITAL_R2)) {
			flip();
		}


		delay(25);

	}
}

// Motor RPM +-200

/*averageDriveSpeed = ( abs(mFrontLeft.get_actual_velocity()) +
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
