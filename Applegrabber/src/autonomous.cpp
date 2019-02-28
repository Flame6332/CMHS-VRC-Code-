#include "main.h"
#include "robotstate.h"
#include "robotconfig.h"
#include "controllibrary.h"

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack u/size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
// remember
//RED = 1;
//BLUE = -1;
void flagSide7pt();
void programmingSkills();

void autonomous() {
  //drive(18, 0.5, 1, 5, 200);
  drive(-5, 0.03, 1, 2, 200);
  rotate(30, 0.5, 10, 30, 200);
  //drive(-18, 0.03, 1, 5, 200);
  //flagSide7pt();
  //programmingSkills();
}


void flagSide7pt() {
  int COLOR = CURRENT_COLOR;
  toggleAutoBallIntake();
  addLoadedBall();
  drive(-24, 5, 1, 4, 200);
  drive(-16, 3, 1, 4, 100);
  drive(24+13.5, 0.5, 1, 4, 200);
  if (COLOR == RED) {
    rotate(-90*COLOR, 0.5, 10, 30, 200);
    drive(-8, 0.5, 1, 2, 200);
  }
  if (COLOR == BLUE) {
    rotate(-85*COLOR, 0.5, 10, 30, 200);
    drive(-5, 0.5, 1, 2, 200);
  }
  primePuncher();
  waitUntilDoneMoving();
  manualFire();
  waitUntilDoneFiring();
  drive(-24, 0.5, 1, 4, 200);
  primePuncher(); // primes while drving
  waitUntilDoneMoving();
  manualFire();
  waitUntilDoneFiring();
  if (COLOR == RED) {
    drive(-15, 4, 1, 4, 200);
    drive(15, 4, 1, 4, 200);
  }
  if (COLOR == BLUE) {
    rotate(190, 2, 10, 30, 200);
    drive(8, 0.5, 1, 4, 200);
    lowerFlippy(); // smacks flag on blue side instead
    raiseFlippy();
    rotate(180, 5, 10, 30, 200);
  }
  rotate(-90*COLOR, 5, 10, 30, 200);
  lowerFlippy();
  drive(10, 10, 1, 4, 200);
  waitUntilDoneMoving();
  raiseFlippy();
}
