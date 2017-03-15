#include "SorticFramework.h"

Detector::Detector(int adress, int target) : Component(adress, target) {

}

bool Detector::recieveMessage(Message transmission) {
  String theMessage = transmission.message;
  theMessage.trim();
  if((_componentState != working)&&(_componentState != idle)) return false;

  if(theMessage.equalsIgnoreCase("scan enable")) {
    _componentState = working;
  }
  else if(theMessage.equalsIgnoreCase("scan disable")) {
    _componentState = idle;
  }
  else return false;

  return true;
}

Mover::Mover(int adress, int target) : Component(adress, target) {

}

bool Mover::recieveMessage(Message transmission) {

  if(_componentState != idle) return false;

  if(transmission.message.equalsIgnoreCase("MoveTo PickUp")) {
    currentTarget = MoverPosition::pickUp;
  }
  else if(transmission.message.equalsIgnoreCase("MoveTo dropA")) {
    currentTarget = MoverPosition::dropA;
  }
  else if(transmission.message.equalsIgnoreCase("MoveTo dropB")) {
    currentTarget = MoverPosition::dropB;
  }
  else if(transmission.message.equalsIgnoreCase("MoveTo dropC")) {
    currentTarget = MoverPosition::dropC;
  }
  else return false;

  _componentState = working;
  return true;
}

Placer::Placer(int adress, int target) : Component(adress, target){

}

bool Placer::recieveMessage(Message transmission) {

  if(_componentState != idle) return false;

  if(transmission.message.equalsIgnoreCase("PickUp Left")) {
    currentPlacerActionType = PlacerActionType::pickUp;
    currentPlacerActionDirection = PlacerActionDirection::left;
  }
  else if(transmission.message.equalsIgnoreCase("PickUp Right")) {
    currentPlacerActionType = PlacerActionType::pickUp;
    currentPlacerActionDirection = PlacerActionDirection::right;
  }
  else if(transmission.message.equalsIgnoreCase("Place Left")) {
    currentPlacerActionType = PlacerActionType::place;
    currentPlacerActionDirection = PlacerActionDirection::left;
  }
  else if(transmission.message.equalsIgnoreCase("Place Right")) {
    currentPlacerActionType = PlacerActionType::place;
    currentPlacerActionDirection = PlacerActionDirection::right;
  }
  else return false;

  _componentState = working;
  return true;

}




SorticController::SorticController(int adress, int target, int mover, int placer, int detector) : Component(adress, target) {
  moverAdress = mover;
  placerAdress = placer;
  detectorAdress = detector;

}

bool SorticController::recieveMessage(Message transmission) {
  if(transmission.sender == detectorAdress) {
    if(!transmission.message.equalsIgnoreCase("error")) {
      currentPartMessage = transmission.message;
      partDetected = true;
      currentPickupDirection = PlacerActionDirection::left;
      currentPlaceDirection = PlacerActionDirection::left;
    }
  }
  else if(transmission.message.equalsIgnoreCase("complete")) {
    if(transmission.sender == moverAdress) moverIsFinished = true;
    else if(transmission.sender == placerAdress) placerIsFinished = true;
    else return false;
  }
  else if(transmission.message.equalsIgnoreCase("begin")) {
    if(_componentState == idle) {
      _componentState = working;
    }
    else return false;
  }
  else if(transmission.message.equalsIgnoreCase("stop")) {
    if(_componentState == idle) return true;
    if(_componentState == working) {
      setNextIdle = true;
    }
    else return false;
  }
  else return false;
  return true;
}

Message SorticController::componentLoop() {
  Message currentMessage;
  currentMessage.hasMessage = false;
  currentMessage.sender = adress;

  if(_componentState == working){
    switch(step) { //1 = enable scan, 2 = drive to pickup, 3 = pickup part, 4 = drive to drop point, 5 = place part, 6 = drive back, decide what to do next
      case 1:
        currentMessage.target = detectorAdress;
        currentMessage.message = "scan enable";
        currentMessage.hasMessage = true;
        step++;
      break;

      case 2:
        if(setNextIdle) {
          step = 1;
          setNextIdle = false;
          _componentState = idle;
          break;
        }
        if(partDetected) {
          currentMessage.target = moverAdress;
          currentMessage.message = "MoveTo PickUp";
          currentMessage.hasMessage = true;
          moverIsFinished = false;
          step++;
        }
      break;

      case 3:
        if(setNextIdle) {
          step = 1;
          setNextIdle = false;
          _componentState = idle;
          break;
        }
        if(moverIsFinished) {
          currentMessage.target = placerAdress;
          if(currentPickupDirection == PlacerActionDirection::left) currentMessage.message = "Pickup Left";
          if(currentPickupDirection == PlacerActionDirection::right) currentMessage.message = "Pickup Right";
          currentMessage.hasMessage = true;
          placerIsFinished = false;
          partDetected = false;
          step++;
        }
      break;

      case 4:
        if(placerIsFinished) {
          currentMessage.target = moverAdress;
          currentMessage.message = "MoveTo DropA";
          currentMessage.hasMessage = true;
          moverIsFinished = false;
          step++;
        }
      break;

      case 5:
        if(moverIsFinished) {
          currentMessage.target = placerAdress;
          if(currentPlaceDirection == PlacerActionDirection::left) currentMessage.message = "Place Left";
          if(currentPlaceDirection == PlacerActionDirection::right) currentMessage.message = "Place Right";
          currentMessage.hasMessage = true;
          placerIsFinished = false;
          step++;
        }
      break;

      case 6:
        if(placerIsFinished) {
          currentMessage.target = moverAdress;
          currentMessage.message = "MoveTo PickUp";
          currentMessage.hasMessage = true;
          moverIsFinished = false;
          step++;
        }
      break;

      case 7:
        if(setNextIdle) _componentState = idle;
        step = 1;
      break;
    }
  }

  return currentMessage;
}
