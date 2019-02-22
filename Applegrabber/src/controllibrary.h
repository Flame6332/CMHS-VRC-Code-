#ifndef CONTROL_LIBRARY_H
#define CONTROL_LIBRARY_H

#include "main.h"
using namespace pros;
using namespace std;

void moveAtVelocity(Motor, int, int, int);
void moveAtVelocityNoStop(Motor, int, int, int);
void stopMotor(Motor);
void waitUntilDoneFiring();


void calibrate();
void flip();
void manualFire();
void primePuncher();
void toggleFlippy();
void toggleBallIntake();
void ballCountUp();
void ballCountDown();
int getCurrentBallCount();

void drive(float, float, float, float, float);
void rotate(float, float, float, float, float);
void waitUntilDoneMoving();
void ramForMiliseconds(int, int);

#endif
