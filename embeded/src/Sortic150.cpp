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
#include "SortingList.h"
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

Component * attachedComponents[5];
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

  //attachedComponents[0] = new DummyDetector(101,1);
  attachedComponents[0] = new DetectorSeverin(101,100,&PartDetector);
  attachedComponents[1] = new MoverSeverin(102,100,DriverMotor, DistanceSensorPin, 510, 400, 300, 200);
  attachedComponents[2] = new PlacerSeverin(103,100,PlacerMotorBase, PlacerMotorArm, PlacerMotorClaw);
  attachedComponents[3] = new SortingList(104,100,"4:186:115:69:162:231:73:128","4:135:115:120:162:231:73:128");
  attachedComponents[4] = new SorticController(100,1,102,103,101,104);//(int adress, int target, int mover, int placer, int detector, int sortingList)

  currentNode = new CommunicationNode(attachedComponents[0], attachedComponents[1], attachedComponents[2], attachedComponents[3], attachedComponents[4], attachedConnections[0]);

  delay(2000);
  /*
  pinMode(13, OUTPUT);
  */

  Message currentMessage;
  currentMessage.hasMessage = true;
  currentMessage.message = "begin";
  currentMessage.target = 100;
  currentMessage.sender = 1;
  currentNode->sendMessage(currentMessage);

  /*
  int pointerAdress = (unsigned int)&attachedComponents[0];
  String output = "adress ";
  output += pointerAdress;
  currentMessage.message = output;// + pointerAdress;
  currentMessage.target = 1;
  currentNode->sendMessage(currentMessage);


  Message currentMessage;
  currentMessage.hasMessage = true;
  currentMessage.message = "mark";
  currentMessage.target = 1;
  currentMessage.sender = 1;

  currentNode->sendMessage(currentMessage);

  Component *componentPointer = attachedComponents[0];

  componentPointer->componentLoop();

  currentNode->sendMessage(currentMessage);

  componentPointer++;

  currentNode->sendMessage(currentMessage);

  if(componentPointer == attachedComponents[1]) {
    while(true);
  }

  currentNode->sendMessage(currentMessage);
  currentNode->sendMessage(currentMessage);
  */
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
