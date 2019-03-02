#include "main.h"
#include "robotstate.h"
#include "robotconfig.h"
#include "controllibrary.h"


using namespace std;
using namespace pros;


bool isFlipping = false;
bool isFlippyEnabled = false;
bool isFlippyRaised = true;
static bool isHalfCarrying = false;

void flip() {
  if (!hasCalibrated) { return; }
  isFlipping = true;
}
void flipAndWait() {
  waitUntilDoneMoving();
  if (!hasCalibrated) { return; }
  isFlipping = true;
  while (isFlipping) {
    delay(5);
  }
}
void beginHalfCarry() {
  isHalfCarrying = true;
}
void endHalfCarry() {
  isHalfCarrying = false;
}

void enableFlippy() {
  isFlippyEnabled = true;
}
void disableFlippy() {
  isFlippyEnabled = false;
}
void toggleFlippy() {
  if (isFlippyEnabled) {
    disableFlippy();
  }
  else {
    enableFlippy();
  }
}
void raiseFlippy() {
  isFlippyRaised = true;
  moveAtVelocityWithTimeOut(mFlippy, -20, 10, 200, 1200);
  mFlippy.move_absolute(-20, 200);
}
void lowerFlippy() {
  isFlippyRaised = false;
  moveAtVelocityWithTimeOut(mFlippy, -185, 10, 200, 1000);
}


static void updateLoop(void* param) {
  calibrate(); // just in case we forgot to flip it up
  //bool shouldSlamDown = false;
  while (666) {
    if (isHalfCarrying) {
        mFlippy.move_absolute(-90, 200);
    }
    else {
        if (isFlippyEnabled && isFlippyRaised) {
          lowerFlippy();
        }
        else if (!isFlippyEnabled && !isFlippyRaised) {
          raiseFlippy();
        }

        if (isFlipping) {
          enableFlippy();
          raiseFlippy(); // upsy
          //if (controlMaster.get_digital(DIGITAL_R2)) {shouldSlamDown = true;}
          //else {shouldSlamDown = false;}
          while (controlMaster.get_digital(DIGITAL_R2)) { // while its being held
            delay(4);
          }
          //if (shouldSlamDown) {moveAtVelocity(mFlippy, 15, 10, 200);}
          //else {moveAtVelocity(mFlippy, 15, 10, 100);}
          lowerFlippy(); //back down
          isFlipping = false;
        }
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
