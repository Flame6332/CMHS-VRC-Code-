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
    be perfect with position, so it's designed to overshoot a bit
    and then recalibrate every once in a while.
*/

bool isFiring = false;
static bool isPrimed = false;
static int primePeriodLength = 4000;
static int primePeriodRemainingTime = 0;

bool isAutoIntakeEnabled = false;
static bool isBallIntakeOn = false;
static bool isPurging = false;
static bool isDoubleBallPurging = false;
static bool isManualPurging = false;

static const int purgePeriodLength = 700; // in miliseconds
static const int manualPurgePeriodLength = 400; // in miliseconds
static const int doubleBallPurgePeriodLength = 900; // in miliseconds
static const int purgePeriodCooldownLength = 200; // in miliseconds
static int purgePeriodRemainingTime = 0;
static int currentBallCount = 0;
static bool hasTopSwitchBeenReleasedYet = false;
static float currentBallDistanceCovered = 0;
static const float maxExpectedSingleBallTravelDistance = 8.3; // around this many inches

static const int primeRotationDistance = 750;
static const int fireRotationDistance = 1160;

static void ballCountDown();

void manualFire() {
  if (!hasCalibrated) {return;}
  isFiring = true;
}
void waitUntilDoneFiring() {
  while (isFiring) {
    delay(5);
  }
}
void manualFireAndWait() { // used in autonomous
  waitUntilDoneMoving(); manualFire(); waitUntilDoneFiring();
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
  mPuncher.move_absolute(primeRotationDistance, 200);
  primePeriodRemainingTime = primePeriodLength;
}
static void endPrime() {
  isPrimed = false;
  stopMotor(mPuncher);
  mPuncher.set_brake_mode(MOTOR_BRAKE_COAST);
}

void turnOnIntake() {
  isBallIntakeOn = true;
  mIntake.move_velocity(200);
}
void reverseIntake() {
  isBallIntakeOn = true;
  mIntake.move_velocity(-200);
}
void turnOffIntake() {
  isBallIntakeOn = false;
  stopMotor(mIntake);
  mIntake.set_brake_mode(MOTOR_BRAKE_BRAKE);
}
static void purge() {
  isPurging = true;
  purgePeriodRemainingTime = purgePeriodLength;
  isDoubleBallPurging = false;
  isManualPurging = false;
}
void manualPurge() {
  isPurging = true;
  isManualPurging = true;
  purgePeriodRemainingTime = manualPurgePeriodLength;
  isDoubleBallPurging = false;
}
static void doubleBallPurge() {
  isPurging = true;
  purgePeriodRemainingTime = doubleBallPurgePeriodLength;
  isDoubleBallPurging = true;
  isManualPurging = false;
}
static void endPurge() {
  isPurging = false;
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
static void liveUpdateDisplayedBallCount() { // lcd can be updated live
  if (hasTopSwitchBeenReleasedYet) {lcd::print(0, "bals: %d topSwitchWasReleased",getCurrentBallCount());}
  else {lcd::print(0, "bals: %d",getCurrentBallCount());}
}
static void updateDisplayedBallCount() { // controller can only be updated once a while
  liveUpdateDisplayedBallCount();
  controlMaster.print(1, 1,"bals %d", getCurrentBallCount()); //to_string(getCurrentBallCount()).c_str());
}
static void ballCountUp() {
  currentBallCount++;
}
void addLoadedBall() {
  hasTopSwitchBeenReleasedYet = true;
  currentBallCount++;
}
static void ballCountDown() {
  if (currentBallCount > 0) {
    currentBallCount--;
  }
  else {
    // return counting error or something
  }
}

static ADIDigitalIn initialIntakeSwitch (4);
  static bool isInitialSwitchPressed = false;
  static bool isInitialSwitchJustPressed = false;
  static bool isInitialSwitchJustReleased = false;
static ADIDigitalIn finalIntakeSwitch (3);
  static bool isFinalSwitchPressed = false;
  static bool isFinalSwitchJustPressed = false;
  static bool isFinalSwitchJustReleased = false;

static void updateSwitchBoolean();
  static const int updateLoopDT = 6; // update loop delta time, number of miliseconds per frame
static void updateLoop(void* param) {
  // wait a second and then make sure balls are zeroed on init
  delay(500);
  currentBallCount = 0;

  while (666) {
    updateSwitchBoolean();

    // keep track of estimated ball count even in manual for switching back
    if (isInitialSwitchJustPressed) {
      ballCountUp();
      updateDisplayedBallCount();
    }

    if (isAutoIntakeEnabled) {

      if (!isPurging) { // if not purging
          if (isInitialSwitchJustPressed) {
            currentBallDistanceCovered = 0; // just started
          }
          else if (isInitialSwitchPressed) {
            // this formula is achieved by taking intake rolling diameter (2.5in) multiplied by pi to
            // convert 1 rotation to 7.85398 chain travel inches, and converting 1 minute to 60000 ms
            // so the formula becomes () rpm * 7.85398 / 60000 ) * number of miliseconds
            // all divided by 2 because the ball moves half the distance of the chain
            currentBallDistanceCovered += (mIntake.get_actual_velocity()*updateLoopDT*0.0001309)/2;
            if (currentBallDistanceCovered > maxExpectedSingleBallTravelDistance) {
              lcd::print(5, "Too far %.4f",currentBallDistanceCovered);
              doubleBallPurge();
            }
          }
          else if (isInitialSwitchJustReleased) {
            lcd::print(5, "%.4f",currentBallDistanceCovered); // print out each balls distance
          }

          // intake on by default, will check all the cases where it might need to be shut off
          turnOnIntake();

          if (isFinalSwitchJustReleased) {
            // if theres 2 balls, and one has passed top switch
            hasTopSwitchBeenReleasedYet = true;
          }

          if (currentBallCount == 2) {
            if (hasTopSwitchBeenReleasedYet && isFinalSwitchPressed) {
              // if one has passed top swtich and the other bumps into top switch
              turnOffIntake();
            }
          }

          if (currentBallCount >= 3) {
            purge(); // something terrible has happened
          }

      }

      else { // isPurging
        if(!isDoubleBallPurging) {lcd::print(3, "we purgin");}
        else {lcd::print(3, "we DOUBLE-BALL purgin");}
        float prePurgePosition = mIntake.get_position();
        mIntake.move_velocity(-200);
        int dt = 5; // change in time
        while (purgePeriodRemainingTime > 0) {
          // still updating switch stuff for this little loop
          updateSwitchBoolean();
          // yeah nah the shooter task is going into
          // lockdown until we get this extra ball out of here.
          delay(dt);

        }

        if (!isManualPurging) { ballCountDown(); } // 3rd ball

        // idk what i'm even doing anymore, at this point, all these edge
        // cases can't cover everything, not to mention im only using
        // 2 switches, I think the best bet is just to leave it as is and
        // add the safety manual mode

        turnOffIntake(); // stop the the intake and then
        while (purgePeriodRemainingTime > -1*purgePeriodCooldownLength) {
          delay(5); //just wait a little longer for ball to roll out
        }

        if (!isDoubleBallPurging) { // the natural reintake process must take place,
          // for doubleball purge as it naturally retriggers bottom swtich since
          // there's still a ball
          // back everything up to pre-purge position
          lcd::print(3, "=> resetting");
          float resetTarget = prePurgePosition + (0.4*(prePurgePosition - mIntake.get_position()));
          moveAtVelocity(mIntake, resetTarget, 7, 200);
        }

        endPurge(); // the chaos is over.
        lcd::print(3, "");
        updateDisplayedBallCount();
      }

    } // end auto intake enabled

    if (isFiring) {
      turnOffIntake();
      if (!isPrimed && isPuncherStopped()) {mPuncher.tare_position();}
      endPrime();
      ballCountDown();
      moveAtVelocityNoStop(mPuncher, fireRotationDistance, 10, 200);
      stopMotor(mPuncher);
      hasTopSwitchBeenReleasedYet = false; // next ball has yet to be released
      isFiring = false;
      updateDisplayedBallCount();
    }
    else if (isPrimed) {
        if (primePeriodRemainingTime <= 0) {
          endPrime();
        }
    }

    delay(updateLoopDT); // response time
  }

}

static void timingLoop(void* param) { // simple external timing loop for accuracy
  int dt = 10; // (delta) (change in time) : miliseconds per loop
  while (1234) {
    primePeriodRemainingTime -= dt;
    purgePeriodRemainingTime -= dt;
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
