#ifndef MoverSeverin_h
#define MoverSeverin_h

#include "Arduino.h"
#include "SorticFramework.h"
#include "filters/MedianFilter.h"
#include <Adafruit_MotorShield.h>

class MoverSeverin : public Mover
{
  public:
    MoverSeverin(int adress, int target, Adafruit_DCMotor *tempDriverMotor, int tempDistanceSensorPin, int PosPickup, int posDropA, int posDropB, int posDropC);
    Message componentLoop();
  private:
    MedianFilter thisMedianFilter;
    bool ForwardIsLeft = true;

    bool isInitialized = false;

    int driveTollerance = 2;
    int maxSpeed = 200;
    int distanceSensorPin;
    Adafruit_DCMotor *DriverMotor;

    //Position Variables
    MoverPosition positionMarked[4] = {MoverPosition::pickUp, MoverPosition::dropA, MoverPosition::dropB, MoverPosition::dropC};
    int positionValues[4];
    int positionMax = 565;   //Maximum sensor value ~= 565, uses max sensor value if larger
    bool bahnHasStopped;
    int rawSensorValue;
    int filteredSensorValue;
    int targetPosition;
    int currentPosition;
    int distanceToTarget;
};

#endif
