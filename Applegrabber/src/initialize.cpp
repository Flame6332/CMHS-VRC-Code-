#include "main.h"
#include "robotstate.h"
#include "robotconfig.h"

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

extern const int RED = 1;
extern const int BLUE = -1;


// PSSST, HEY YOU, PLEASE, OVER HERE
// CHANGE THE CURRENT COLOR BEFORE EACH MATCH
extern const int CURRENT_COLOR = RED;




 Motor mFrontLeft (1);
 Motor mFrontRight (2, true);
 Motor mBackLeft (3);
 Motor mBackRight (4, true);

 Motor mIntake (5, true);
 Motor mPuncher (6);
 Motor mFlippy (17, true);
 Motor mRam (8, true);

 Vision visionSensor (16);

 Controller controlMaster(CONTROLLER_MASTER);
 Controller controlPartner(CONTROLLER_PARTNER);

bool hasCalibrated;

void calibrate() {
		mFlippy.tare_position();
		//mPuncher.tare_position();
		mRam.tare_position();
		hasCalibrated = true;
}

void startFlippyTask();
void startAutoFireTask();
void startFiringTask();
void startAutonomousMovementTask();

void initialize() {

	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");
	pros::lcd::register_btn1_cb(on_center_button);

	mFrontLeft.set_encoder_units(MOTOR_ENCODER_DEGREES);
	mFrontRight.set_encoder_units(MOTOR_ENCODER_DEGREES);
	mBackLeft.set_encoder_units(MOTOR_ENCODER_DEGREES);
	mBackRight.set_encoder_units(MOTOR_ENCODER_DEGREES);
	mIntake.set_encoder_units(MOTOR_ENCODER_DEGREES);
	mPuncher.set_encoder_units(MOTOR_ENCODER_DEGREES);
	mFlippy.set_encoder_units(MOTOR_ENCODER_DEGREES);
	mRam.set_encoder_units(MOTOR_ENCODER_DEGREES);

	mRam.tare_position();
	mPuncher.tare_position();
	mFlippy.set_zero_position(190);
	hasCalibrated = true;

	mFlippy.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
	startFlippyTask();
	startAutoFireTask();
	startFiringTask();
	startAutonomousMovementTask();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}
