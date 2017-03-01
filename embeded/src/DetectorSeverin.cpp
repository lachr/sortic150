#include "Arduino.h"
#include "SorticFramework.h"
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

#include "DetectorSeverin.h"

DetectorSeverin::DetectorSeverin(int adress, int target, MFRC522 *tempPartDetector) : Detector(adress, target) {
  PartDetector = *tempPartDetector;
  PartDetector.PCD_Init();

}

Message DetectorSeverin::componentLoop() {
  Message currentMessage;

  if(_componentState != working) {
    currentMessage.hasMessage = false;
    return currentMessage;
  }

  if (!PartDetector.PICC_IsNewCardPresent()) {
    currentMessage.hasMessage = false;
    return currentMessage;
  }

  currentMessage.hasMessage = true;

  byte blockAddr = 0;
  byte bufferSize = 18;
  MFRC522::StatusCode status;
  byte partArray[8];

  status = (MFRC522::StatusCode) PartDetector.MIFARE_Read(blockAddr, partArray, &bufferSize);

  if(status != MFRC522::STATUS_OK) {
    currentMessage.message = "error";
    return currentMessage;
  }
  else {
    currentMessage.message = partArray[0];
    for(unsigned int i = 1 ; i<sizeof(partArray)/sizeof(partArray[0]); i++) {
      currentMessage.message = ":" + currentMessage.message + partArray[i];
    }
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
