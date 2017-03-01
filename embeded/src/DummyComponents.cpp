#include "Arduino.h"
#include "SorticFramework.h"
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

#include "DummyComponents.h"

DummyDetector::DummyDetector(int adress, int target) : Detector(adress, target) {

}

Message DummyDetector::componentLoop() {
  Message currentMessage;

  if(_componentState != working) {
    currentMessage.hasMessage = false;
    return currentMessage;
  }



  if(millis()%10000==1) {
    currentMessage.hasMessage = true;
    currentMessage.message = millis();
    _componentState = idle;
  }


  currentMessage.sender = adress;
  currentMessage.target = target;
  return currentMessage;
}

//Error detection
/*
if(status == MFRC522::STATUS_COLLISION){
  for (int i = 0; i < 8; i++) {
    partArray[i] = 1;
  }
}
else if(status == MFRC522::STATUS_CRC_WRONG){
  for (int i = 0; i < 8; i++) {
    partArray[i] = 2;
  }
}
else if(status == MFRC522::STATUS_ERROR){
  for (int i = 0; i < 8; i++) {
    partArray[i] = 3;
  }
}
else if(status == MFRC522::STATUS_INTERNAL_ERROR){
  for (int i = 0; i < 8; i++) {
    partArray[i] = 4;
  }
}
else if(status == MFRC522::STATUS_INVALID){
  for (int i = 0; i < 8; i++) {
    partArray[i] = 5;
  }
}
else if(status == MFRC522::STATUS_MIFARE_NACK){
  for (int i = 0; i < 8; i++) {
    partArray[i] = 6;
  }
}
else if(status == MFRC522::STATUS_NO_ROOM){
  for (int i = 0; i < 8; i++) {
    partArray[i] = 7;
  }
}
else if(status == MFRC522::STATUS_TIMEOUT){
  for (int i = 0; i < 8; i++) {
    partArray[i] = 8;
  }
}
else {
  for (int i = 0; i < 8; i++) {
    partArray[i] = 0;
  }
}
*/
