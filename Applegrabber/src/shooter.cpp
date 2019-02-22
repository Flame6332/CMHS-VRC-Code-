#include "main.h"
#include "robotstate.h"
#include "robotconfig.h"
#include "controllibrary.h"


using namespace std;
using namespace pros;


bool isFiring = false;
bool isPrimed = false;
int primeUpdateLoopsRemaining = 0;
bool isBallIntaking = false;
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
void primePuncher() {
  if (!isPrimed) { // if its a fresh prime, reset position
    mPuncher.tare_position();
  }
  isPrimed = true;
  mPuncher.move_absolute(450, 200);
  primeUpdateLoopsRemaining = 700;
}
static void endPrime() {
  isPrimed = false;
  stopMotor(mPuncher);
}

static void enableIntake() {
  isBallIntaking = true;
  mIntake.move_velocity(200);
}
static void disableIntake() {
  isBallIntaking = false;
  stopMotor(mIntake);
  mIntake.set_brake_mode(MOTOR_BRAKE_BRAKE);
}
void toggleBallIntake() {
  if (!isBallIntaking) {
    enableIntake();
  } else {
    disableIntake();
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

    if (isBallIntaking) {
      mIntake.move_velocity(200);
    }

    if (isInitialSwitchJustPressed) {
      ballCountUp();
      if (currentBallCount >= 2) {
        disableIntake();
        lcd::print(0, "%d",getCurrentBallCount());
        controlMaster.clear_line(0);
        controlMaster.set_text(0, 0, ""+getCurrentBallCount());
      }
    }
    if (isInitialSwitchJustReleased) {
      controlMaster.clear_line(0);
      controlMaster.set_text(0, 0, ""+getCurrentBallCount());
    }

    if (isFiring) {
      disableIntake();
      if (!isPrimed) {mPuncher.tare_position();}
      primeUpdateLoopsRemaining = 0; // clear it out
      mLift.set_brake_mode(MOTOR_BRAKE_HOLD);
      mPuncher.set_brake_mode(MOTOR_BRAKE_COAST);
      moveAtVelocityNoStop(mLift, 100, 10, 200);
      //printf("John \n");
      endPrime();
      ballCountDown();
      moveAtVelocityNoStop(mPuncher, 1160, 20, 200);
      stopMotor(mPuncher);
      //printf("dfhn \n");
      moveAtVelocityNoStop(mLift, 25, 10, 200);
      mLift.set_brake_mode(MOTOR_BRAKE_BRAKE);
      enableIntake();
      isFiring = false;
    } else if (isPrimed) {
        if (primeUpdateLoopsRemaining > 0) {
          primeUpdateLoopsRemaining--;
        }
        else {
          endPrime();
        }
    }

    delay(6); // response time
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
}
