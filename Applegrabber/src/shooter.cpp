#include "main.h"
#include "robotstate.h"
#include "robotconfig.h"
#include "controllibrary.h"


using namespace std;
using namespace pros;

/*
  NOTE:
    The ball shooters relative position gets slowly corrupted over
    time due to the presence of the slip gear making it impossible to
    be perfect with position, so it's designed to overshoot sometimes
*/

bool isFiring = false;
bool isPrimed = false;
int primePeriodLength = 1000;
int primePeriodRemainingTime = 0;
bool isAutoIntakeEnabled = false;
bool isBallIntakeOn = false;
int currentBallCount = 1; // preload 1 ball

void manualFire() {
  if (!hasCalibrated) {return;}
  isFiring = true;
}
void waitUntilDoneFiring() {
  while (isFiring) {
    delay(5);
  }
}
void manualFireAndWait() {
  manualFire(); waitUntilDoneFiring();
}
bool isPuncherStopped() {
  if (abs(mPuncher.get_actual_velocity()) < 2) {
    return true;
  } else {return false;}
}
void primePuncher() {
  // if its not currently primed, and not still sliding after the end of a previous slide, reset position
  if (!isPrimed && isPuncherStopped()) {
    mPuncher.tare_position();
  }
  isPrimed = true;
  mPuncher.move_absolute(600, 200);
  primePeriodRemainingTime = primePeriodLength;
}
static void endPrime() {
  isPrimed = false;
  stopMotor(mPuncher);
}

static void turnOnIntake() {
  isBallIntakeOn = true;
  mIntake.move_velocity(200);
}
static void turnOffIntake() {
  isBallIntakeOn = false;
  stopMotor(mIntake);
  mIntake.set_brake_mode(MOTOR_BRAKE_BRAKE);
}
void toggleAutoBallIntake() {
  if (!isAutoIntakeEnabled) {
    isAutoIntakeEnabled = true;
  } else {
    isAutoIntakeEnabled = false;
  }
}

int getCurrentBallCount() {
  return currentBallCount;
}
void ballCountUp() {
  currentBallCount++;
}
void ballCountDown() {
  if (currentBallCount > 0) {
    currentBallCount--;
  }
}

static ADIDigitalIn initialIntakeSwitch (4);
  static bool isInitialSwitchPressed = false;
  static bool isInitialSwitchJustPressed = false;
  static bool isInitialSwitchJustReleased = false;
static ADIDigitalIn finalIntakeSwitch (5);
  static bool isFinalSwitchPressed = false;
  static bool isFinalSwitchJustPressed = false;
  static bool isFinalSwitchJustReleased = false;

static void updateSwitchBoolean();

static void updateLoop(void* param) {
  while (666) {

    updateSwitchBoolean();

    lcd::print(0, "%d",getCurrentBallCount());

    if (isAutoIntakeEnabled) {

        turnOnIntake();
        if (isInitialSwitchJustPressed) {
          ballCountUp();
          if (currentBallCount >= 2) {
            turnOffIntake();
            lcd::print(0, "%d",getCurrentBallCount());
            controlMaster.clear_line(0);
            controlMaster.set_text(0, 0, ""+getCurrentBallCount());
          }
        }
        if (isInitialSwitchJustReleased) {
          controlMaster.clear_line(0);
          controlMaster.set_text(0, 0, ""+getCurrentBallCount());
        }

    }

    if (isFiring) {
      turnOffIntake();
      if (!isPrimed && isPuncherStopped()) {mPuncher.tare_position();}
      endPrime();
      mPuncher.set_brake_mode(MOTOR_BRAKE_COAST);
      moveAtVelocityNoStop(mLift, 100, 10, 200);  //printf("John \n");
      endPrime();
      ballCountDown();
      moveAtVelocityNoStop(mPuncher, 1160, 20, 200);
      stopMotor(mPuncher); //printf("dfhn \n");  //moveAtVelocityNoStop(mLift, 25, 10, 200); mLift.set_brake_mode(MOTOR_BRAKE_BRAKE);
      turnOnIntake();
      isFiring = false;
    }
    else if (isPrimed) {
        if (primePeriodRemainingTime <= 0) {
          endPrime();
        }
    }

    delay(6); // response time
  }

}

static void timingLoop(void* param) { // simple external timing loop
  int dt = 10; // (delta) (change in time) : miliseconds per loop
  while (1234) {
    primePeriodRemainingTime -= dt;
    delay(dt);
  }
}


static void updateSwitchBoolean() {
  isInitialSwitchJustPressed = false;
  isInitialSwitchJustReleased = false;
  if (!isInitialSwitchPressed && initialIntakeSwitch.get_value()) {
    isInitialSwitchJustPressed = true;
  } else if (isInitialSwitchPressed && !initialIntakeSwitch.get_value()) {
    isInitialSwitchJustReleased = true;
  }
  isInitialSwitchPressed = initialIntakeSwitch.get_value();

  isFinalSwitchJustPressed = false;
  isFinalSwitchJustReleased = false;
  if (!isFinalSwitchPressed && finalIntakeSwitch.get_value()) {
    isFinalSwitchJustPressed = true;
  } else if (isFinalSwitchPressed && !finalIntakeSwitch.get_value()) {
    isFinalSwitchJustReleased = true;
  }
  isFinalSwitchPressed = finalIntakeSwitch.get_value();
}

void startFiringTask() {
  Task fireTask(updateLoop, (void*)"PROS", TASK_PRIORITY_DEFAULT,
                TASK_STACK_DEPTH_DEFAULT, "fireTask");
  Task fireTimingTask(timingLoop, (void*)"PROS", TASK_PRIORITY_DEFAULT,
                TASK_STACK_DEPTH_DEFAULT, "fireTimingTask");
}
