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
void standardFlagSide();
void programmingSkills();

void autonomous() {
  //drive(18, 0.5, 1, 5, 200);
  //drive(-5, 0.03, 1, 2, 200);
  //rotate(360, 0.5, 10, 30, 200);
  //drive(-18, 0.03, 1, 5, 200);
  standardFlagSide();
  //programmingSkills();
}


void standardFlagSide() {
  int COLOR = CURRENT_COLOR;
  toggleAutoBallIntake();
  addLoadedBall();
  drive(-30, 12, 1, 4, 200);
  drive(-17, 3, 0.4, 7, 120);
  drive(30+15, 10, 1, 4, 200);
  ramForMiliseconds(200, 210); // reset on wall
  primePuncher();
  if (COLOR == RED) {
    drive(-3, 0.4, 1, 1.5, 200);
    rotate(-90*COLOR, 0.5, 10, 30, 200);
    drive(-3, 0.5, 1, 2, 200);
  }
  if (COLOR == BLUE) {
    drive(-13, 0.4, 1, 1.5, 200);
    rotate(-90*COLOR, 0.5, 10, 30, 200);
    drive(-3, 0.5, 1, 2, 200);
  }
  manualFireAndWait();
  drive(-24, 0.5, 1, 4, 200);
  primePuncher(); // primes while drving
  manualFireAndWait();
  if (COLOR == RED) {
    drive(-15, 4, 1, 4, 200);
    drive(15, 4, 1, 4, 200);
  }
  if (COLOR == BLUE) {
    rotate(180, 2, 10, 40, 200);
    drive(8, 5, 1, 4, 200);
    waitUntilDoneMoving();
    flip(); // KNOCK THE FLAG, FLIP AND WAIT WILL LOWER SINCE ALREADY DISABLED
    delay(400);
    disableFlippy();
    rotate(-180, 5, 10, 30, 200);
  }

  rotate(60*COLOR, 5, 10, 30, 200);
  drive(-48, 10, 1, 4,200);

  //drive(10, 10, 1, 4, 200);
  //waitUntilDoneMoving();
  //raiseFlippy();

}

void farSideOverPlatform() {
  int COLOR = CURRENT_COLOR;
  toggleAutoBallIntake();
  addLoadedBall();
}
