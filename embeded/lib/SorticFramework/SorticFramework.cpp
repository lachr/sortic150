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
      transmission.message = currentPartMessage;
      if(_componentState == idle) _componentState = working;
    }
  }
  else if(transmission.message.equalsIgnoreCase("complete")) {
    if(transmission.sender == moverAdress) moverIsFinished = true;
    if(transmission.sender == placerAdress) placerIsFinished = true;
    if(moverIsFinished && placerIsFinished) _componentState = idle;
  }
  else return false;
  return true;
}
