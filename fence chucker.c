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

void pre_auton()

{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
  // Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
					bStopTasksBetweenModes = true; 

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

  // DEFAULT LIFT AND DROP SPEEDS!!!
  float lift1Speed = 50;
  float lift2Speed = 45;
  float lift3Speed = 50;
    float drop1Speed = -25;
    float drop2Speed = -33;
    float drop3Speed = -33;

  void liftAll(float speed) {
    /*set all motor ports to tuned speeds*/
      motor[lift1] = (int) lift1Speed * speed;
      motor[lift2] = (int) lift2Speed * speed;
      motor[lift3] = (int) lift3Speed * speed;
  }

  void stopAll() {
    motor[lift1] = 0;
    motor[lift2] = 0;
    motor[lift3] = 0;
  }

  void dropAll(float speed) {
    /*set all motor ports to tuned speeds*/
      motor[lift1] = (int) drop1Speed * speed;
      motor[lift2] = (int) drop2Speed * speed;
      motor[lift3] = (int) drop3Speed * speed;
  }

//// BOOOLEANS ---- there is no booleans so 0 is false, 1 is true
    // if its currently lifting
      int isLifting = 0;

    //if the trim button was just clicked
      bool trim1JustClicked = false;
      bool trim2JustClicked = false;
      bool trim3JustClicked = false;

  //Lift mode (1 is auto)(0 is manual)
  int liftMode = 0;

  //LIFT MULTIPLIER, can be 1, 1.5, or 2
  float speed = 1;

int fineTuneUp = 50;
int fineTuneDown = -30;

//////////////////////////////////////////////////////////////////////////////////////////
task autonomous()
{

/*  motor[rightdrive] = 90;
  motor[leftdrive] = 90;

  wait1Msec(6000);

  motor[rightdrive] = 90;
  motor[leftdrive] =  30;

  wait1Msec(2000);

  motor[rightdrive] = 70;
  motor[leftdrive] = 70;

  wait1Msec(6000);

  motor[rightdrive] = 0;
  motor[leftdrive] = 0;

  liftAll(1.5);

  wait1Msec(6000);

  stopAll();
*/
}

task usercontrol() {
/*infinite loop*/
while (69 == 69) //lol, 69, first number that came to mind
{

  /*move the wheels*/
  motor[rightdrive] = vexRT[Ch2] / 2;
  motor[leftdrive] = vexRT[Ch3] / 2;


  ///// STOP ALL MOTORS INSTANTLY COMMAND
  if (vexRT[Btn8R] == 1) {
    lift1Speed = 50;
  	lift2Speed = 50;
  	lift3Speed = 50;
    drop1Speed = -15;
    drop2Speed = -33;
    drop3Speed = -33;
  }

  //// CHANGE SPEED COMMAND
  if (vexRT[Btn5U] == 1) {
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


  /////////////////////////////////////////////////////////
  //########## BASIC RAISE AND LOWER FUNCTIONS ##########//
  //----------------------------------------------------//

    /////AUTOMATIC LIFT MODE
    if (liftMode == 1) {

        ///RAISE COMMAND///
        if (vexRT[Btn7U] == 1 && isLifting == 0)
        {
            liftAll(speed);
            isLifting = 1; //set lifting to true
              /*wait 10000 seconds then attempt to stop all motors*/
              wait1Msec(10000 / speed);
                if (isLifting != 0) {
                  stopAll();
                  isLifting = 0; //set lifting to false because its over
                }
        }

        ///LOWER COMMAND///
        if (vexRT[Btn7D] == 1 && isLifting == 0)
        {
            dropAll(speed);
            isLifting = 1; //set lifting to true
              /*wait 10000 seconds then attempt to stop all motors*/
              wait1Msec(7000 / speed);
                if (isLifting != 0) {
                  stopAll();
                  isLifting = 0; //set lifting to false because its over
                }
        }

    }

    else {
    /////MANUAL LIFT MODE

        ///RAISE COMMAND///
        if (vexRT[Btn7U] == 1) {
          liftAll(speed);
          isLifting = 1; //set lifting to true
        }
        ///LOWER COMMAND///
        else if (vexRT[Btn7D] == 1) {
          dropAll(speed);
          isLifting = 1; //set lifting to true
        }
        else if (isLifting == 1) {
            stopAll();
            isLifting = 0; //set lifting to false because its over
        }

    }

  //-----------------------------------------------------//
  //________END OF BASIC RAISE AND LOWER FUNCTIONS_______//
  /////////////////////////////////////////////////////////


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@//


  /////////////////////////////////////////////////////////
  //############## FINE TUNING FUNCTIONS ################//
  //-----------------------------------------------------//

  ///// if is lifting is false : fine tuning allowed
  if (isLifting == 0) {

  		//lift1 is single motor//
  		//                     //
  		//                     //
  		//                     //
  		//lift2 and lift3 multi//


        ////// manual and auto mode
      //if (vexRT[Btn6D] == 1 && vexRT[Btn8U] == 1) {
        //liftMode = 1;
      //}
      //lift2 trim click
          if (vexRT[Btn7L] == 1 && vexRT[Btn8U] == 1) { // if trim-up button and fine tune button
              lift1Speed += 0.1;
          } else if (vexRT[Btn7R] == 1 && vexRT[Btn8U] == 1) {
              drop1Speed -= 0.1;
          }
      /// lift2 fine tune
      else if (vexRT[Btn8U] == 1 && vexRT[Btn6U] == 1) { motor[lift1] = fineTuneDown + 10; }
      /// lift2 fine tune
      else if (vexRT[Btn8U] == 1) { motor[lift1] = fineTuneUp - 10; }
      //stop left2
      else {
        motor[lift1] = 0;
      }

      //lift2 trim click
          if (vexRT[Btn7L] == 1 && vexRT[Btn8L] == 1) { // if trim-up button and fine tune button
              lift2Speed += 0.1;
          } else if (vexRT[Btn7R] == 1 && vexRT[Btn8L] == 1) {
              drop2Speed -= 0.1;
          }
      /// lift2 fine tune
      else if (vexRT[Btn8L] == 1 && vexRT[Btn6U] == 1) { motor[lift2] = fineTuneDown; }
      /// lift2 fine tune
      else if (vexRT[Btn8L] == 1) { motor[lift2] = fineTuneUp; }
      //stop left2
      else {
        motor[lift2] = 0;
      }

      //lift2 trim click
          if (vexRT[Btn7L] == 1 && vexRT[Btn8D] == 1) { // if trim-up button and fine tune button
              lift3Speed += 0.1;
          } else if (vexRT[Btn7R] == 1 && vexRT[Btn8D] == 1) {
              drop3Speed -= 0.1;
          }
      /// lift2 fine tune
      else if (vexRT[Btn8D] == 1 && vexRT[Btn6U] == 1) { motor[lift3] = fineTuneDown; }
      /// lift2 fine tune
      else if (vexRT[Btn8D] == 1) { motor[lift3] = fineTuneUp; }
      //stop left2
      else {
        motor[lift3] = 0;
      }

  }

  //-----------------------------------------------------//
  //____________END OF FINE TUNING FUNCTIONS_____________//
  /////////////////////////////////////////////////////////

}
}
