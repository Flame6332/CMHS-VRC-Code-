#include "main.h"
#include "robotstate.h"
#include "robotconfig.h"

using namespace std;
using namespace pros;

void flip() {

}

void startTasks() {
  Task flipTask(updateLoop, (void*)"PROS", TASK_PRIORITY_DEFAULT,
                TASK_STACK_DEPTH_DEFAULT, "My Task");)
}

void updateLoop(void* param) {

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
