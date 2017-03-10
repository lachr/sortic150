#include "MoverSeverin.h"

#include "Arduino.h"
#include "SorticFramework.h"

MoverSeverin::MoverSeverin(int adress, int target, Adafruit_DCMotor *tempDriverMotor, int tempDistanceSensorPin, int PosPickup, int posDropA, int posDropB, int posDropC) : Mover(adress, target) {
  positionValues[0] = PosPickup;
  positionValues[1] = posDropA;
  positionValues[2] = posDropB;
  positionValues[3] = posDropC;
  DriverMotor = tempDriverMotor;
  distanceSensorPin = tempDistanceSensorPin;
  rawSensorValue = analogRead(distanceSensorPin);
  thisMedianFilter = MedianFilter(rawSensorValue);

  DriverMotor->run(FORWARD);    //FORWARD = Nach Rechts, BACKWARD = Nach Links
  DriverMotor->run(RELEASE);
  DriverMotor->setSpeed(0);

  /*
  if(targetPosition > positionMax) {
    targetPosition = positionMax;
  }
  */
}

Message MoverSeverin::componentLoop() {
  Message currentMessage;

  if(_componentState != working) {
    currentMessage.hasMessage = false;
    return currentMessage;
  }

  if(!isInitialized) {
    isInitialized = true;
    for(int i = 0; i<4;i++) {
      if(positionMarked[i]==currentTarget) {
        targetPosition = positionValues[i];
      }

    }
    if(targetPosition > positionMax) {
      targetPosition = positionMax;
    }
  }

  rawSensorValue = analogRead(distanceSensorPin);
  thisMedianFilter.UpdateFilter(rawSensorValue);
  filteredSensorValue = thisMedianFilter.getFilterValue();

  if(bahnHasStopped == true) {
    DriverMotor->run(RELEASE);
  }

  if((bahnHasStopped == false)and(filteredSensorValue != 0))
  {
    //Funktion fuer Uebertragung in nutzbare grösse
    int currentPosition = filteredSensorValue;

    //In richtige Richtung fahren
    if(currentPosition>targetPosition) {
      //Drive left
      if(ForwardIsLeft) {
        DriverMotor->run(FORWARD);
      }
      else {
        DriverMotor->run(BACKWARD);
      }
    }
    else {
      //Drive right
      if(ForwardIsLeft) {
        DriverMotor->run(BACKWARD);
      }
      else {
        DriverMotor->run(FORWARD);
      }
    }

    distanceToTarget = abs(currentPosition-targetPosition);

    //Set Speed relative to target
    if(distanceToTarget > 150) {
      DriverMotor->setSpeed(maxSpeed);
    }
    else{
      DriverMotor->setSpeed(distanceToTarget+70);
    }

    //Stop at target
    //driveTollerance = distanceToTarget/200+1; //100->2, 200->3, 300->4, 400->5, 500->6
    if(distanceToTarget<driveTollerance) {
      DriverMotor->setSpeed(0);
      currentMessage.hasMessage = true;
      currentMessage.message = "complete";
      currentMessage.target = target;
      currentMessage.sender = adress;
      isInitialized = false;
      _componentState = idle;
    }
  }


  return currentMessage;
}
