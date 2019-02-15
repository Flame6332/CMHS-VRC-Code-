#ifndef CONTROL_LIBRARY_H
#define CONTROL_LIBRARY_H

#include "main.h"
using namespace pros;
using namespace std;

void moveAtVelocity(Motor, int, int, int);
void stopMotor(Motor);
void drive(float, float, float, float, float);
void rotate(float, float, float, float, float);

#endif
