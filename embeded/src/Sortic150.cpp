#include <Arduino.h>


#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

#include "SorticFramework.h"

//#include "SorticMachineSeverin.h"
#include "MoverSeverin.h"
#include "PlacerSeverin.h"
#include "DetectorSeverin.h"


#include "DummyComponents.h"
#include "SorticFramework.h"
#include "SerialConnection.h"
#include "Modular.h"


//Sensors:
int DistanceSensorPin = A1;

//Motors:

Adafruit_MotorShield currentMotorShield = Adafruit_MotorShield();
Adafruit_DCMotor *DriverMotor = currentMotorShield.getMotor(1);
Adafruit_DCMotor *PlacerMotorBase = currentMotorShield.getMotor(2);
Adafruit_DCMotor *PlacerMotorArm = currentMotorShield.getMotor(3);
Adafruit_DCMotor *PlacerMotorClaw = currentMotorShield.getMotor(4);

//RFID Detectors:
MFRC522 PartDetector(6,5);

//Components:
DetectorSeverin *currentDetector;
MoverSeverin *currentMover;
PlacerSeverin *currentPlacer;
DummyDetector *myDummyDetector;
//SorticMachineSeverin *thisSorticMachine;

SerialConnection *USBConnection;

CommunicationNode *currentNode;

Component * attachedCompoents[2];
CommunicationConnection * attachedConnections[1];

int adressListConnection1[1] = {1};


/*
Adresses:
  Controller = 1
  Detector = 101
  Mover = 102
  Placer = 103
*/


void setup() {

  attachedConnections[0] = new SerialConnection(&adressListConnection1[0], sizeof(adressListConnection1), 9600);

  currentMotorShield.begin();
  SPI.begin();

  //attachedCompoents[0] = new DummyDetector(101,1);
  attachedCompoents[0] = new DetectorSeverin(101,1,&PartDetector);
  //attachedCompoents[1] = new MoverSeverin(102,1,DriverMotor, DistanceSensorPin, 510, 400, 300, 200);
  attachedCompoents[1] = new PlacerSeverin(103,1,PlacerMotorBase, PlacerMotorArm, PlacerMotorClaw);

  currentNode = new CommunicationNode(attachedCompoents[0], sizeof(attachedCompoents)/sizeof(attachedCompoents[0]), attachedConnections[0], sizeof(attachedConnections)/sizeof(attachedConnections[0]));

  delay(2000);

  pinMode(13, OUTPUT);

}

void loop() {
  /*
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(100);
  */
  currentNode->loopAllAttached();

}
