/////////////////
///
///  NOTE: this code is not functional as the variables are only random examples.
///
////////////////






#pragma config(Motor,  port1,           frontLeftWheel,    		tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port2,           frontRightWheel,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           rearLeftWheel,         	tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           rearRightWheel,     	tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           leftArmLift1,         	tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           leftArmLift2,    		tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           leftArmLift3,         	tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           rightArmLift1,         	tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           rightArmLift1,     		tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          rightArmLift1,         	tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port11,          leftClaw,         		tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port12,          rightClaw,         		tmotorVex393_MC29, openLoop)

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

float exampleDecimalVariable = 50;

bool aTrueOrFalseVariable = false;

int exampleInteger = 13;

void exampleFunction(float exampleInputVariable) {
    
  motor[driveTrainLeft] = (int) lift1Speed * speed; //changing the float (decimal) to an integer (whole number)
    
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

    motor[rightdrive] = 90; //motor speed ranges from 127 to -127
    motor[leftdrive] = 90;

    wait1Msec(6000); //wait 6 seconds

    motor[rightdrive] = 90; //setting the motor to the specified speed
    motor[leftdrive] =  30;

    wait1Msec(2000);

    motor[rightdrive] = 70;
    motor[leftdrive] = 70;

    wait1Msec(6000);

    motor[rightdrive] = 0;
    motor[leftdrive] = 0;

    liftAll(1.5);

    wait1Msec(6000);

    stopAll(); //user created function

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
while (69 == 69) //lol, get rekt
{

    /*move the wheels*/
    motor[rightdrive] = vexRT[Ch2];
    //this will set the motor from -127 to 127 depending on the position of the stick's channer
    motor[leftdrive] = vexRT[Ch3]; 

  if (vexRT[Btn8R] == 1) {//if this button is clicked (aka = to 0)
  
      motor[liftDrive] = 60 * speed; //multiply by speed variable for easy changing of speed
 
  }

  //Change Speed Command//
  if (vexRT[Btn5U] == 1) { //while this button is held down, click following buttons to change speed

        if (vexRT[Btn7D] == 1) { //normal low (1) speed
          speed = 1;
        }
        else if (vexRT[Btn7L] == 1) { //faster medium (1.5) speed
          speed = 1.5;
        }
        else if (vexRT[Btn7U] == 1) { //turbo (2) speed
          speed = 2;
        }

  }
    
    //robot trimming code
    if (vexRT[Btn5D] == 1) {
        speed += 0.1;    //speed = speed + 0.1
    }

}
}
