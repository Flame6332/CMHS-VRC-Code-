#include "main.h"
#include "robotstate.h"
#include "robotconfig.h"


using namespace std;
using namespace pros;


void moveAtVelocity(Motor motor, int target, int tolerance, int rpm) {
  motor.move_absolute(target, rpm);
  while (!( (motor.get_position() < target+(tolerance*2)) && (mFlippy.get_position() > target-(tolerance*2)) )) {
    pros::delay(tolerance*2); // delay loop until increased accuracy
  }
  while (!( (motor.get_position() < target+tolerance) && (mFlippy.get_position() > target-tolerance) )) {
    pros::delay(tolerance); // delay loop with increased accuracy
  }
}
