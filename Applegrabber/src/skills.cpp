#include "main.h"
#include "robotstate.h"
#include "robotconfig.h"
#include "controllibrary.h"

void programmingSkills() {
  toggleBallIntake();
  manualFire();
  waitUntilDoneFiring();
  drive(-49, 0.5, 1, 4, 200);
  //if (getCurrentBallCount() == 1) {
    drive(25, 0.5, 1, 4, 200);
    primePuncher();
    waitUntilDoneMoving();
    manualFire(); waitUntilDoneFiring();
  //}
  drive(72, 0.5, 1, 4, 200);
  rotate(90, 0.5, 10, 30, 200);
  ramForMiliseconds(-200, 200);



}
