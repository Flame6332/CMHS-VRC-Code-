#include "main.h"
#include "robotstate.h"
#include "robotconfig.h"
#include "controllibrary.h"

void programmingSkills() {
  toggleAutoBallIntake();
  addLoadedBall();

  primePuncher();
  drive(-24, 30, 1, 1, 200); //
  manualFireAndWait(); // first shot

  drive(-20, 100, 1, 1, 200);
  ramForMiliseconds(-200, 200); // ram extra reliability and force
  drive(48, 5, 1, 3, 200); // back up for high flag
  primePuncher(); // prime on way there
  manualFireAndWait(); // fire at middle flag

  drive(48, 20, 1, 4, 200); // drive to far side
  rotate(90, 20, 10, 30, 200);
  ramForMiliseconds(200, 500); // ram into wall flipper side to calibrate

  drive(-56, 30, 1, 3, 200); // ram cap over and eat ball
  rotate(-90, 20, 10, 30, 200); // rotate around to flip cap
  lowerFlippy(); // lower flippy as rotating
  waitUntilDoneMoving();
  raiseFlippy();  // flip cap





}
