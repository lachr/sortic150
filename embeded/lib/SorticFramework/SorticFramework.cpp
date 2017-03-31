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




SorticController::SorticController(int adress, int target, int mover, int placer, int detector, int sortingList) : Component(adress, target) {
  moverAdress = mover;
  placerAdress = placer;
  detectorAdress = detector;
  sortingListAdress = sortingList;
}

bool SorticController::recieveMessage(Message transmission) {
  if(transmission.sender == detectorAdress) {
    if(!transmission.message.equalsIgnoreCase("error")) {
      currentPartMessage = transmission.message;
      partDetected = true;
      currentPickupDirection = "left";
      currentPickupPosition = "PickUp";
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
  else if(transmission.sender == sortingListAdress) {
    int sepparator =  transmission.message.indexOf(':');
    currentPlacePosition = transmission.message.substring(0,sepparator);
    currentPlaceDirection = transmission.message.substring(sepparator+1,transmission.message.length());
    partIdentified = true;
  }
  else return false;
  return true;
}

Message SorticController::componentLoop() {
  Message currentMessage;
  currentMessage.hasMessage = false;
  currentMessage.sender = adress;

  if(_componentState == working){
    switch(step) { //1 = enable scan, 2 = get orders, 3 = drive to pickup, 4 = pickup part, 5 = drive to drop point, 6 = place part, 7 = drive back, 8 = decide what to do next
      case 1:
        currentMessage.target = detectorAdress;
        currentMessage.message = "scan enable";
        currentMessage.hasMessage = true;
        partDetected = false;
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
          currentMessage.target = sortingListAdress;
          currentMessage.message = currentPartMessage;
          currentMessage.hasMessage = true;
          partIdentified = false;
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
        if(partIdentified) {
          partIdentified = false;
          currentMessage.target = moverAdress;
          currentMessage.message = "MoveTo " + currentPickupPosition;
          currentMessage.hasMessage = true;
          moverIsFinished = false;

          step++;
        }
      break;

      case 4:
        if(setNextIdle) {
          setNextIdle = false;
          step = 1;
          _componentState = idle;
          break;
        }
        if(moverIsFinished) {
          currentMessage.target = placerAdress;
          currentMessage.message = "Pickup " + currentPickupDirection;
          currentMessage.hasMessage = true;
          placerIsFinished = false;
          partDetected = false;
          step++;
        }
      break;

      case 5:
        if(placerIsFinished) {
          currentMessage.target = moverAdress;
          currentMessage.message = "MoveTo " + currentPlacePosition;
          currentMessage.hasMessage = true;
          moverIsFinished = false;
          step++;
        }
      break;

      case 6:
        if(moverIsFinished) {
          currentMessage.target = placerAdress;
          currentMessage.message = "Place "+ currentPlaceDirection;
          currentMessage.hasMessage = true;
          placerIsFinished = false;
          step++;
        }
      break;

      case 7:
        if(placerIsFinished) {
          currentMessage.target = moverAdress;
          currentMessage.message = "MoveTo " + currentPickupPosition;
          currentMessage.hasMessage = true;
          moverIsFinished = false;
          step++;
        }
      break;

      case 8:
        if(moverIsFinished) {
          if(setNextIdle) _componentState = idle;
          step = 1;
        }
      break;
    }
  }

  return currentMessage;
}
