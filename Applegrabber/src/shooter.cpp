#include "main.h"
#include "robotstate.h"
#include "robotconfig.h"
#include "controllibrary.h"


using namespace std;
using namespace pros;


bool isFiring = false;

void manualFire() {
  if (!hasCalibrated) {return;}
  isFiring = true;
}

static void updateLoop(void* param) {
  while (666) {

    if (isFiring) {
      mLift.set_brake_mode(MOTOR_BRAKE_HOLD);
      mPuncher.set_brake_mode(MOTOR_BRAKE_COAST);
      moveAtVelocity(mLift, 100, 10, 200);
      moveAtVelocity(mPuncher, 1140, 20, 200);
      moveAtVelocity(mLift, 5, 10, 200);
      mLift.set_brake_mode(MOTOR_BRAKE_BRAKE);
      isFiring = false;
    }
    delay(6); // response time
  }

}

void startFiringTask() {
  Task fireTask(updateLoop, (void*)"PROS", TASK_PRIORITY_DEFAULT,
                TASK_STACK_DEPTH_DEFAULT, "fireTask");
}
