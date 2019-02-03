#include "main.h"
#include "robotstate.h"
#include "robotconfig.h"
#include "controllibrary.h"


using namespace std;
using namespace pros;


bool isFlipping = false;

void flip() {
  if (!hasCalibrated) {return;}
  isFlipping = true;
}

static void updateLoop(void* param) {
  while (666) {

    if (isFlipping) {
      moveAtVelocity(mFlippy, 180, 10, 200);
      isFlipping = false;
    }
    delay(10); // response time
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
