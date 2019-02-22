#include "main.h"
#include "robotstate.h"
#include "robotconfig.h"
#include "controllibrary.h"

void programmingSkills() {
  toggleAutoBallIntake();

  manualFireAndWait(); // first shot, match start

  drive(-44, 100, 1, 1, 200); // ram bottom flag and pick up ball
  ramForMiliseconds(-200, 200); // ram extra reliability and force
  drive(25, 5, 1, 3, 200); // back up for middle flag
  primePuncher(); // prime on way there
  waitUntilDoneMoving();
  manualFireAndWait(); // fire at middle flag

  drive(72, 20, 1, 4, 200); // drive to far side
  rotate(-90, 20, 10, 30, 200);
  ramForMiliseconds(-200, 200); // ram into wall flipper side to calibrate

  drive(-45, 30, 1, 3, 200); // ram cap over and eat ball
  rotate(-90, 20, 10, 30, 200); // rotate around to flip cap
  moveAtVelocity(mFlippy, 15, 10, 200); // lower flippy as rotating
  waitUntilDoneMoving();
  moveAtVelocity(mFlippy, 180, 10, 200); // flip cap





}
