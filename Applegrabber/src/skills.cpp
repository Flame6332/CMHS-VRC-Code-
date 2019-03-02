#include "main.h"
#include "robotstate.h"
#include "robotconfig.h"
#include "controllibrary.h"

void programmingSkills() {
  toggleAutoBallIntake();
  addLoadedBall();

  primePuncher();
  drive(-24, 10, 1, 4, 200); //
  manualFireAndWait(); // first shot
  delay(600);
  drive(-20, 40, 1, 1, 200);
//  ramForMiliseconds(-200, 400); // ram extra reliability and force
  drive(48, 0.9, 1, 4, 300); // back up for high flag
  primePuncher(); // prime on way there
  manualFireAndWait(); // fire at middle flag

  drive(50, 20, 1, 4, 200); // drive to far side
  rotate(90, 1, 10, 30, 200);
  ramForMiliseconds(200, 600); // ram into wall flipper side to calibrate

  // GOOD



  drive(-41.5, 0.5, 1, 3, 200); // ram cap over and eat ball
  drive(24, 1, 1, 4, 200);
  rotate(-90, 0.9, 10, 30, 200);


  rotate(-90, 0.5, 10, 30, 200);
  drive(18.5, 1, 1, 4, 200);
  rotate(-90, 0.5, 10, 30, 200);
  drive(87, 1, 1, 4, 200);
  rotate(-90, 0.5, 10, 30, 200);
  drive(18.5, 1, 1, 4, 200);
  rotate(90, 0.5, 10, 30, 200);
  drive(20, 1, 1, 4, 200);
  ramForMiliseconds(200, 400);


  /*rotate(-120, 2, 10, 20, 200); // rotate around to flip cap
  enableFlippy();
  drive(5.5, 1, 1, 1, 200);
  flipAndWait();
  rotate(-60, 3, 10, 10, 200);
  drive(11, 3, 1, 2, 200);
  rotate(45, 3, 10, 10, 200);
  drive(6, 1, 1, 1, 200);
  flipAndWait();
  rotate(-60, 3, 10, 10, 200);
  disableFlippy();

  drive(50, 20, 1, 3, 200);
  ramForMiliseconds(200, 500); // ram into wall flipper side to calibrate/**/

  drive(-41, 20, 1, 2, 200);
  drive(38, 40, 1, 2, 200);
  ramForMiliseconds(200, 300); // ram into wall flipper side to calibrate

  drive(-6, 30, 1, 2, 200);
  rotate(90, 20, 10, 20, 200);
  drive(-49, 20, 1, 4, 200);
  primePuncher();
  manualFireAndWait();
  drive(-24, 20, 1, 3, 200);
  primePuncher();
  manualFireAndWait();


}
