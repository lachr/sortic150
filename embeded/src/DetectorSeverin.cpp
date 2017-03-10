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
  _componentState = idle;
  currentMessage.hasMessage = true;
  currentMessage.sender = adress;
  currentMessage.target = target;

  byte blockAddr = 0;
  byte bufferSize = 18;
  MFRC522::StatusCode status;
  byte partArray[18];

  status = (MFRC522::StatusCode) PartDetector.MIFARE_Read(blockAddr, partArray, &bufferSize);

  if(status != MFRC522::STATUS_OK) {
    switch(status) {
      default:  // ignore Default STATUS_OK
      break;

      case MFRC522::STATUS_COLLISION:
      currentMessage.message = "STATUS_COLLISION";
      break;

      case MFRC522::STATUS_CRC_WRONG:
      currentMessage.message = "STATUS_CRC_WRONG";
      break;

      case MFRC522::STATUS_ERROR:
      currentMessage.message = "STATUS_ERROR";
      break;

      case MFRC522::STATUS_INTERNAL_ERROR:
      currentMessage.message = "STATUS_INTERNAL_ERROR";
      break;

      case MFRC522::STATUS_INVALID:
      currentMessage.message = "STATUS_INVALID";
      break;

      case MFRC522::STATUS_MIFARE_NACK:
      currentMessage.message = "STATUS_MIFARE_NACK";
      break;

      case MFRC522::STATUS_NO_ROOM:
      currentMessage.message = "STATUS_NO_ROOM";
      break;

      case MFRC522::STATUS_TIMEOUT:
      currentMessage.message = "STATUS_TIMEOUT";
      break;
    }
    currentMessage.message = "error" + currentMessage.message;

    return currentMessage;
  }
  else {
    currentMessage.message = partArray[0];
    for(unsigned int i = 1 ; i<8/*sizeof(partArray)/sizeof(partArray[0])*/; i++) {
      currentMessage.message = currentMessage.message + ":" + partArray[i];
    }
  }
  return currentMessage;
}
