#include "SortingList.h"

SortingList::SortingList(int adress, int target, String part1, String part2) : Component(adress, target) {
  this->part1 = part1;
  this->part2 = part2;
}

Message SortingList::componentLoop() {
  if(!messageSent) {
    nextMessage.hasMessage = true;
    nextMessage.sender = adress;
    nextMessage.target = target;
    messageSent = true;
  }
  else nextMessage.hasMessage = false;
  return nextMessage;
}


bool SortingList::recieveMessage(Message transmission) {
  if(transmission.message.equalsIgnoreCase(part1)) {
    nextMessage.message = "dropA:Left";
    messageSent = false;
  }
  else if(transmission.message.equalsIgnoreCase(part2)) {
    nextMessage.message = "dropB:Left";
    messageSent = false;
  }
  else {
    nextMessage.message = "PickUp:Right";
    messageSent = false;
  }
  return true;
}
