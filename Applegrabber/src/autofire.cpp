#include "main.h"
#include "robotstate.h"
#include "robotconfig.h"
#include "controllibrary.h"
#include <cmath>

using namespace std;
using namespace pros;

// VISION_FOV_WIDTH = 316
// VISION_FOV_HEIGHT = 212
// 0,0 top left corner default
// positive x is right
// postiive y is down

bool isAutoDriving = false;
bool isAutoFiring = false;

static const float PI = 3.141592654;

static const float X_AIM_POINT = 158; // 316 / 2fvf
static const float Y_AIM_POINT = 106; // 212 / 2

static const int RED_FLAG_MARKER_SIG = 1;
static const int BLUE_FLAG_MARKER_SIG = 2;
static const int YELLOW_FLAG_MARKER_SIG = 3; // signature id

static const int NUM_OF_YELLOW_FLAG_MARKERS_TRACKED = 4;

static vision_object_s_t yellowFlagMarkersArray[NUM_OF_YELLOW_FLAG_MARKERS_TRACKED]; // this is an array

void autoAim() {
  isAutoFiring = true;
}
void cancelAutoAim() {
  cancelAndStopMovement();
  isAutoDriving = false;
  isAutoFiring = false;
}

static float xMidCoord(vision_object_s_t visObj) {
  return visObj.left_coord + 0.5 * visObj.width;
}
static float yMidCoord(vision_object_s_t visObj) {
  return visObj.top_coord + 0.5 * visObj.height;
}
static int rnd(float x) {
  return (int) ( x + 0.5 - (x<0) );
}
static float toDeg(float x) {
  return (180./PI) * x;
}
static float absDist(float x1, float y1, float x2, float y2) {
  return sqrt(pow(x2-x1, 2) + pow(y2-y1, 2));
}

static float flagColorOffset(vision_object_s_t visObj) {
  float offset = (visObj.height * 0.5) * CURRENT_COLOR;
  if (offset < 8) { offset = 8; }
  return offset;
}
static float getYellowFlagMarkerTargetX(vision_object_s_t visObj) {
  return (xMidCoord(visObj) + flagColorOffset(visObj));
}
static float getYellowFlagMarkerTargetY(vision_object_s_t visObj) {
  return (yMidCoord(visObj));
}
static float getYellowFlagMarkerTargetDeltaX(vision_object_s_t visObj) {
  return getYellowFlagMarkerTargetX(visObj) - X_AIM_POINT;
}
static float getYellowFlagMarkerTargetDeltaY(vision_object_s_t visObj) {
  return getYellowFlagMarkerTargetY(visObj) - Y_AIM_POINT;
}
static float getYellowMarkerTargDistFromTarg(vision_object_s_t visObj) {
  return absDist(getYellowFlagMarkerTargetX(visObj), getYellowFlagMarkerTargetY(visObj), X_AIM_POINT, Y_AIM_POINT);
}

static void updateLoop(void* param) {
  float deltaX = 0;
  float deltaY = 0;
  const float rotationConst = 274;
  const float DIST_CONST = 0.55;
  const float ANGLE_TOLERANCE = 3; // degrees
  const float DIST_TOLERANCE = 1; // inches
  float distanceRequired = 0;
  float rotationRequired = 0;
  int objCount = 0;
  float closestObjDist = 0;
  int closestObjNum = 0;


  // the rotation constant is a result of placing an object to the very edge
  // of the field of view of the camera, measuring the trangular distance of that object
  // from the camera, and then scaling those values to pixels, so it is now known,
  // that if an object is in the very edge of field of view of the camera's 35 degree
  // range, that it will be 158 pixels (the opposite) from the the midpoint, and theoretically
  // 231 pixels or so away parallel to the robot (the adjacent)

  while (666) {

    if (isAutoFiring) {
      primePuncher();
      isAutoDriving = true;


      if (visionSensor.get_object_count() == 0) {
        cancelAutoAim();
        lcd::set_text(1, "No Objects Detected");
      }
      else {

              // 0 means largest object[0] first
              objCount =
                visionSensor.read_by_sig(0, YELLOW_FLAG_MARKER_SIG,
                  NUM_OF_YELLOW_FLAG_MARKERS_TRACKED, yellowFlagMarkersArray);
              closestObjNum = 0;



            closestObjDist = getYellowMarkerTargDistFromTarg( yellowFlagMarkersArray[0] );
            closestObjNum = 0;

            if (objCount > 1) {
              for ( int i = 1; i < objCount; i++ ) {
                  float i_ObjTargDistFromTarg = getYellowMarkerTargDistFromTarg( yellowFlagMarkersArray[i] );
                  printf(" i is %i  objCount is %i", i, objCount);
                  if (i_ObjTargDistFromTarg < closestObjDist) {
                    closestObjDist = i_ObjTargDistFromTarg;
                    closestObjNum = i;
                  }
              }
            }

            deltaX = getYellowFlagMarkerTargetDeltaX(yellowFlagMarkersArray[closestObjNum]);
            deltaY = getYellowFlagMarkerTargetDeltaY(yellowFlagMarkersArray[closestObjNum]);

          //  deltaX = (xMidCoord(yellowFlagMarkersArray[closestObjNum]) + flagColorOffset) - X_AIM_POINT;
          //  deltaY = yMidCoord(yellowFlagMarkersArray[closestObjNum]) - Y_AIM_POINT;

            //int flagColorOffset = (yellowFlagMarkersArray[0].height * 0.5f) * CURRENT_COLOR;
          ///  if (flagColorOffset < 8) { flagColorOffset = 8; }

            rotationRequired = atan(deltaX / rotationConst) * (180./PI);
            distanceRequired = -deltaY * DIST_CONST;

            lcd::set_text(1, "X_TARG: " + to_string(rnd(xMidCoord(yellowFlagMarkersArray[0]))) +
              "  Y_TARG: " + to_string(rnd(yMidCoord(yellowFlagMarkersArray[0]))) +
              "  ROT: " + to_string(rnd(rotationRequired)) +
              "  CNT: " + to_string(visionSensor.get_object_count()));
            lcd::print(2, "DT_DIST: %0.3f  ", distanceRequired);
            printf("X_TARG: %0.1f Y_TARG: %0.1f ROT: %0.2f OBJCOUNT: %d  \n",
              xMidCoord(yellowFlagMarkersArray[0]),
              yMidCoord(yellowFlagMarkersArray[0]),
              rotationRequired,
              visionSensor.get_object_count());

            if (abs(rotationRequired) > ANGLE_TOLERANCE) { rotate(rotationRequired, 2, 5, 15, 200); }
            if (abs(distanceRequired) > DIST_TOLERANCE) { drive(distanceRequired, 2, 1, 2, 200); }

            waitUntilDoneMoving();


        }

      isAutoDriving = false;
      isAutoFiring = false;

    }

    delay(6); // response time
  }

}

void startAutoFireTask() {
  Task autoFireTask(updateLoop, (void*)"PROS", TASK_PRIORITY_DEFAULT,
                TASK_STACK_DEPTH_DEFAULT, "autoFireTask");
}
