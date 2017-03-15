#include "SortingList.h"

SortingList::SortingList(int adress, int target, String part1, String pat2) : Component(adress, target) {
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
    nextMessage.message = "PosA:Left";
    messageSent = false;
  }
  else if(transmission.message.equalsIgnoreCase(part2)) {
    nextMessage.message = "PosB:Right";
    messageSent = false;
  }
  else {
    nextMessage.message = "PosA:Right";
    messageSent = false;
  }
  return true;
}
