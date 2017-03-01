#include "SorticFramework.h"

//Constructor



Placer::Placer(int adress, int target) : Component(){
  this->adress = adress;
  this-> target = target;

}

bool Placer::recieveMessage(int sender, String message) {

  if(_componentState != idle) return false;

  if(message.equalsIgnoreCase("PickUp Left")) {
    currentPlacerActionType = PlacerActionType::pickUp;
    currentPlacerActionDirection = PlacerActionDirection::left;
  }
  else if(message.equalsIgnoreCase("PickUp Right")) {
    currentPlacerActionType = PlacerActionType::pickUp;
    currentPlacerActionDirection = PlacerActionDirection::right;
  }
  else if(message.equalsIgnoreCase("Place Left")) {
    currentPlacerActionType = PlacerActionType::place;
    currentPlacerActionDirection = PlacerActionDirection::left;
  }
  else if(message.equalsIgnoreCase("Place Right")) {
    currentPlacerActionType = PlacerActionType::place;
    currentPlacerActionDirection = PlacerActionDirection::right;
  }
  else return false;

  _componentState = working;
  return true;

}

Mover::Mover(int adress, int target) : Component() {
  this->adress = adress;
  this-> target = target;
}

bool Mover::recieveMessage(int sender, String message) {

  if(_componentState != idle) return false;

  if(message.equalsIgnoreCase("MoveTo PickUp")) {
    currentTarget = MoverPosition::pickUp;
  }
  else if(message.equalsIgnoreCase("MoveTo dropA")) {
    currentTarget = MoverPosition::dropA;
  }
  else if(message.equalsIgnoreCase("MoveTo dropB")) {
    currentTarget = MoverPosition::dropB;
  }
  else if(message.equalsIgnoreCase("MoveTo dropC")) {
    currentTarget = MoverPosition::dropC;
  }
  else return false;

  _componentState = working;
  return true;
}

Detector::Detector(int adress, int target) : Component() {
  this->adress = adress;
  this-> target = target;
}

bool Detector::recieveMessage(int sender, String message) {
  String theMessage = message;
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

SorticController::SorticController(int adress, int target, int mover, int placer, int detector) : Component() {
  this->adress = adress;
  this-> target = target;
  moverAdress = mover;
  placerAdress = placer;
  detectorAdress = detector;
}

bool SorticController::recieveMessage(int sender, String message) {
  if(sender == detectorAdress) {
    if(!message.equalsIgnoreCase("error")) {
      message = currentPartMessage;
      if(_componentState == idle) _componentState = working;
    }
  }
  else if(message.equalsIgnoreCase("complete")) {
    if(sender == moverAdress) moverIsFinished = true;
    if(sender == placerAdress) placerIsFinished = true;
    if(moverIsFinished && placerIsFinished) _componentState = idle;
  }
  else return false;
  return true;
}
