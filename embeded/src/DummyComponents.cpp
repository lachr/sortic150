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

  if(millis()%20==1) {
    currentMessage.hasMessage = true;
    currentMessage.message = millis();
    _componentState = idle;
  }
  else {
    currentMessage.hasMessage = false;
    return currentMessage;
  }

  currentMessage.sender = adress;
  currentMessage.target = target;
  return currentMessage;
}
