#include "main.h"
#include "robotstate.h"
#include "robotconfig.h"


using namespace std;
using namespace pros;

void resetWheelEncoders() {
  mFrontLeft.tare_position();
  mFrontRight.tare_position();
  mBackLeft.tare_position();
  mBackRight.tare_position();
}

void stopMotor(Motor motor) {
  motor.move_velocity(0);
}

void moveAtVelocity(Motor motor, int target, int tolerance, int rpm) {
  motor.move_absolute(target, rpm);
  while (!( (motor.get_position() < target+(tolerance*2)) && (motor.get_position() > target-(tolerance*2)) )) {
    pros::delay(tolerance*2); // delay loop until increased accuracy
  }
  while (!( (motor.get_position() < target+tolerance) && (motor.get_position() > target-tolerance) )) {
    pros::delay(tolerance); // delay loop with increased accuracy
  }
  stopMotor(motor);
}
bool moveAtVelocityWithTimeOut(Motor motor, int target, int tolerance, int rpm, int miliseconds) {
  motor.move_absolute(target, rpm);
  int timeRemaining = miliseconds;
  while (!( (motor.get_position() < target+(tolerance*2)) && (motor.get_position() > target-(tolerance*2)) )) {
    delay(tolerance*2); // delay loop until increased accuracy
    timeRemaining -= tolerance*2;
    if (timeRemaining < 0) { goto endOfMoveAtVelocityWithTimeOut;  }
  }
  while (!( (motor.get_position() < target+tolerance) && (motor.get_position() > target-tolerance) )) {
    delay(tolerance); // delay loop with increased accuracy
    timeRemaining -= tolerance;
    if (timeRemaining < 0) { goto endOfMoveAtVelocityWithTimeOut;  }
  }
  endOfMoveAtVelocityWithTimeOut:
  stopMotor(motor);
  return timeRemaining < 0; // true if timed out
}

void moveAtVelocityNoStop(Motor motor, int target, int tolerance, int rpm) {
  motor.move_absolute(target, rpm);
  while (!( (motor.get_position() < target+(tolerance*2)) && (motor.get_position() > target-(tolerance*2)) )) {
    pros::delay(tolerance*2); // delay loop until increased accuracy
  }
  while (!( (motor.get_position() < target+tolerance) && (motor.get_position() > target-tolerance) )) {
    pros::delay(tolerance); // delay loop with increased accuracy
  }
}

/// AUTONOMOUS OR AUTO-DRIVE ONLY FUNCTIONS ///

static float wheelDiameter = 4.13;
// converts inches to degrees of rotation
static float distToWheelRot(float distance) {
  return (distance / (wheelDiameter * 3.141592654)) * 360;
}

static void setMovementVelocity(float);

static bool isDoneMoving = true;
static float currentMaxStoppingSpeed = 0;
// absolute rotational distance in drive, and absolute rot dist in rotate function
static float generalAbsoluteWheelRotationTarget = 0;

static float accelerationDistance = 0;
static float accelerationWheelRot = distToWheelRot(accelerationDistance);
static float deccelerationDistance = 0;
static float deccelerationWheelRot = distToWheelRot(deccelerationDistance);
static const float accelerationFactor = 0.5; // deccel too

static float currentLeftSideMotorTarget = 0;
static float currentRightSideMotorTarget = 0;
static float currentSetSpeed = 0;
static bool isRotating = false;

void drive(float distance, float maxStoppingSpeed, float accelerationDist, float deccelerationDist, float speed) {
  while (!isDoneMoving) {
    delay(3); // delay execution until previous movement complete
  }
  resetWheelEncoders();
  isRotating = false;
  float target = distToWheelRot(distance);
  generalAbsoluteWheelRotationTarget = abs(target);
  currentMaxStoppingSpeed = maxStoppingSpeed;
  currentLeftSideMotorTarget = target;
  currentRightSideMotorTarget = target;
  currentSetSpeed = speed;
  accelerationDistance = accelerationDist;
  accelerationWheelRot = distToWheelRot(accelerationDistance);
  deccelerationDistance = deccelerationDist;
  deccelerationWheelRot = distToWheelRot(deccelerationDistance);
  mFrontLeft.move_absolute(target, speed);
  mFrontRight.move_absolute(target, speed);
  mBackLeft.move_absolute(target, speed);
  mBackRight.move_absolute(target, speed);
  isDoneMoving = false;
}
void rotate(float rotationDistance, float maxStoppingSpeed, float accelerationDist, float deccelerationDist, float speed) {
  while (!isDoneMoving) {
    delay(3); // delay execution until previous movement complete
  }
  resetWheelEncoders();
  isRotating = true;
  float rotationalConst = 3.8;
  float target = rotationDistance * rotationalConst;
  generalAbsoluteWheelRotationTarget = abs(target);
  currentMaxStoppingSpeed = maxStoppingSpeed;
  currentLeftSideMotorTarget = target;
  currentRightSideMotorTarget = -target;
  currentSetSpeed = speed;
  accelerationDistance = 0;
  accelerationWheelRot = accelerationDist*rotationalConst;
  deccelerationDistance = 0;
  deccelerationWheelRot = deccelerationDist*rotationalConst;
  mFrontLeft.move_absolute(target, speed);
  mFrontRight.move_absolute(-target, speed);
  mBackLeft.move_absolute(target, speed);
  mBackRight.move_absolute(-target, speed);
  isDoneMoving = false;
}

static float averageAbsoluteVelocity() {
  float fL =  abs( mFrontLeft.get_actual_velocity() );
  float fR =  abs( mFrontRight.get_actual_velocity() );
  float bL =  abs( mBackLeft.get_actual_velocity() );
  float bR =  abs( mBackRight.get_actual_velocity() );
  return ( fL + fR + bL + bR ) / 4;
}
static float averageAbsoluteTargetPositionError() {
  float fLErr =  abs( mFrontLeft.get_position() - currentLeftSideMotorTarget );
  float bLErr =  abs( mBackLeft.get_position() - currentLeftSideMotorTarget );
  float fRErr =  abs( mFrontRight.get_position() - currentRightSideMotorTarget );
  float bRErr =  abs( mBackRight.get_position() - currentRightSideMotorTarget );
  return ( fLErr + fRErr + bLErr + bRErr ) / 4;
}
static float averageAbsolutePosition() {
  float fL =  abs( mFrontLeft.get_position() ); // front left
  float fR =  abs( mFrontRight.get_position() );
  float bL =  abs( mBackLeft.get_position() );
  float bR =  abs( mBackRight.get_position() );
  return ( fL + fR + bL + bR ) / 4;
}

static void setMovementVelocity(float rpm) {
  mFrontRight.move_absolute(currentRightSideMotorTarget, rpm);
  mBackRight.move_absolute(currentRightSideMotorTarget, rpm);
  mFrontLeft.move_absolute(currentLeftSideMotorTarget, rpm);
  mBackLeft.move_absolute(currentLeftSideMotorTarget, rpm);
}

void ramForMiliseconds(int velocity, int miliseconds) {
  while (!isDoneMoving) {
    delay(4);
  }
  mFrontRight.move_velocity(velocity);
  mBackRight.move_velocity(velocity);
  mFrontLeft.move_velocity(velocity);
  mBackLeft.move_velocity(velocity);
  delay(miliseconds);
  mFrontRight.move_velocity(0);
  mBackRight.move_velocity(0);
  mFrontLeft.move_velocity(0);
  mBackLeft.move_velocity(0);
}

static bool shouldBeAccelerating() {
  if (averageAbsolutePosition() < accelerationWheelRot ||
  averageAbsolutePosition() > (generalAbsoluteWheelRotationTarget - deccelerationWheelRot) ) {
      return true;
    }
  else {return false;}
}

static bool isCloseToTarget() {
  if (generalAbsoluteWheelRotationTarget < 4) {
        // if there's 50% of the trip left
        return ((averageAbsoluteTargetPositionError() / generalAbsoluteWheelRotationTarget) < 0.50);
  }
  else {
    return (averageAbsoluteTargetPositionError() < 3); // it has to be within x inches
  }
}

static void updateLoop(void* param) {
  while (69) {
    while (!isDoneMoving) {


        if (shouldBeAccelerating()) {setMovementVelocity(accelerationFactor*currentSetSpeed);}
        else {setMovementVelocity(currentSetSpeed);}
        // if acceptably stopped and atleast 80% of the way to the target, then consider if stopped moving
        // this was added because the robot backed into my feet because it assumed it was done at the first move,
        // since it was stopped moving at the beginning.
        if (averageAbsoluteVelocity() < currentMaxStoppingSpeed
            && isCloseToTarget()) {
              /*printf("AverageAbsoluteVelocity: %.3f \n", averageAbsoluteVelocity());
              printf("CurrentMaxStoppingSpeed: %.3f \n", currentMaxStoppingSpeed);
              printf("AverageAbsoluteTargetPositionError: %.3f \n", averageAbsoluteTargetPositionError());
              printf("generalAbsoluteWheelRotationTarget: %.3f \n\n", generalAbsoluteWheelRotationTarget);*/
          isDoneMoving = true;
        } else {
          /*printf("motorGetPosition: %.3f  motorGetTargetPosition: %.3f \n", mFrontLeft.get_position(), currentLeftSideMotorTarget);
          printf("AverageAbsoluteVelocity: %.3f  CurrentMaxStoppingSpeed: %.3f \n", averageAbsoluteVelocity(), currentMaxStoppingSpeed);
          printf("AverageAbsoluteTargetPositionError: %.3f   generalAbsoluteWheelRotationTarget: %.3f \n", averageAbsoluteTargetPositionError(), generalAbsoluteWheelRotationTarget);
          /**/
        }

        delay(40);

    }
    delay(10);
  }
}

void waitUntilDoneMoving() {
  while (!isDoneMoving) {
    delay(5);
  }
}

void cancelAndStopMovement() {
  mFrontRight.move_velocity(0);
  mBackRight.move_velocity(0);
  mFrontLeft.move_velocity(0);
  mBackLeft.move_velocity(0);
  isDoneMoving = true;
}

void startAutonomousMovementTask() {
  Task autonMoveTask(updateLoop, (void*)"PROS", TASK_PRIORITY_DEFAULT,
                TASK_STACK_DEPTH_DEFAULT, "autonMoveTask");
}
