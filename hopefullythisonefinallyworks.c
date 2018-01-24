#pragma config(Motor,  port1,           wheelFrontLeft, tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           wheelFrontRight, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           wheelRearLeft, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           wheelRearRight, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           liftLeftSide,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           liftRightSide, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           coneFlipLeft,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           coneFlipRight, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           claw,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          mobileGoalLift, tmotorVex393_HBridge, openLoop)

#pragma platform(VEX)

//Competition Control and Duration Settings// DON'T MODIFY!
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!




/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////
void pre_auton() {
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
  // Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
					bStopTasksBetweenModes = true;

	// All activities that occur before the competition starts
	// Example: setting servo positions, extending arm, loading shotgun ...
    // If your robot is a Transformer(copyrighted), then transformation occurs here

}





/////////////////////////////////////////////////////////////////////////////////////////
//
//                             Used Variables and Funcions
//
// Any variables or functions that you will be using are initialized in this area right here.
//
/////////////////////////////////////////////////////////////////////////////////////////

int vectorFrontLeft = 0;
int vectorFrontRight = 0;
int vectorRearLeft = 0;
int vectorRearRight = 0;

void resetWheelVectors() {
	vectorFrontLeft = 0;
	vectorFrontRight = 0;
	vectorRearLeft = 0;
	vectorRearRight = 0;
}

void runWheels() {
	motor[wheelFrontLeft] = vectorFrontLeft;
	motor[wheelFrontRight] = vectorFrontRight;
	motor[wheelRearLeft] = vectorRearLeft;
	motor[wheelRearRight] = vectorRearRight;
}




/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////
task autonomous() {

	// rip

}




/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////
task usercontrol() {
/* an infinite loop */
while (0 == 0) // lol it looks like a face
{

	resetWheelVectors();
	
	vectorFrontLeft += vexRT[Ch3];
	vectorFrontRight += vexRT[Ch3];
	vectorRearLeft += vexRT[Ch3];
	vectorRearRight += vexRT[Ch3];
	
	vectorFrontLeft += vexRT[Ch1];
	vectorFrontRight -= vexRT[Ch1];
	vectorRearLeft += vexRT[Ch1];
	vectorRearRight -= vexRT[Ch1];
	
	runWheels();
	
	
	
}
}
