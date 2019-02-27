#include "main.h"
#include "robotstate.h"
#include "robotconfig.h"
#include "controllibrary.h"


using namespace std;
using namespace pros;


bool isFlipping = false;
bool isRetracted = true;
bool shouldToggleUpdate = false; // i have no idea what this does, i'd probably just leave it though

void flip() {
  if (!hasCalibrated) {return;}
  isFlipping = true;
}

void toggleFlippy() {
  if (isRetracted) {
    isRetracted = false;
  }
  else {
    isRetracted = true;
  }
}

static void updateLoop(void* param) {
  bool shouldSlamDown = false;
  while (666) {

    if (isFlipping) {
      moveAtVelocity(mFlippy, 180, 10, 200);
      if (controlMaster.get_digital(DIGITAL_R2)) {shouldSlamDown = true;}
      else {shouldSlamDown = false;}
      while (controlMaster.get_digital(DIGITAL_R2)) { // while its being held
        delay(4);
      }
      if (shouldSlamDown) {moveAtVelocity(mFlippy, 15, 10, 200);}
      else {moveAtVelocity(mFlippy, 15, 10, 100);}
      isFlipping = false;
    }
    delay(6); // response time
  }

}

void startFlippyTask() {
  Task flipTask(updateLoop, (void*)"PROS", TASK_PRIORITY_DEFAULT,
                TASK_STACK_DEPTH_DEFAULT, "flipperTask");
}




/*class FlipperControl {
    Motor mFlippy;
  public:
    FlipperControl(Motor motor) : mFlippy(motor)  {
      Task flipperTask(updateLoop, (void*)"fa", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "A task");
    }
    bool isFlipping = false;

    void flip() {
        if (!hasCalibrated) {return;}
        isFlipping = true;
        //rotateTo(180);
    }

    void (FlipperControl::*updateLoop) (void* param) {

    };

};*/


/// GAFF tape
