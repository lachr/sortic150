#include "Modular.h"
/*
void Message::switchAdresses() {
  int a = target;
  target = sender;
  sender = a;
}
*/

//some code if needed

//Component::Component(int newAdress, int newTarget) {
Component::Component(int adress, int target) {
  this->adress = adress;
  this->target = target;
}

void Component::setInterrupt(bool enableInterrupt) {
  if(enableInterrupt) {
    stateBeforeInterrupt = _componentState;
    _componentState = interrupt;
  }
  else {
    if(_componentState == interrupt) {
      _componentState = stateBeforeInterrupt;
    }
  }
}


int Component::getAdress() {
  return adress;
}

//CommunicationConnection::CommunicationConnection(int adress) { //(&newAttachedAdresses)[10]
CommunicationConnection::CommunicationConnection(int *adressList, unsigned int numberOfAdresses) { //(&newAttachedAdresses)[10]
 this->adressList = adressList;
 this->numberOfAdresses = numberOfAdresses;
}

bool CommunicationConnection::hasConnectionAttached(int connection){
  for(unsigned int i = 0; i < numberOfAdresses; i++) {
    if(*(adressList+i)==connection) return true;
  }
  return false;

}

//CommunicationNode::CommunicationNode() {

CommunicationNode::CommunicationNode(Component *componentList, unsigned int numberOfComponents, CommunicationConnection *communicationList, unsigned int numberOfConnections) {
  this->componentList = componentList;
  this->numberOfComponents = numberOfComponents;
  this->communicationList = communicationList;
  this->numberOfConnections = numberOfConnections;
}


void CommunicationNode::setInterrput(bool enableInterrupt) {
  /*
  for(unsigned int i = 0; i < sizeof(attachedComponents)/sizeof(attachedComponents[0]); i++) {
    attachedComponents[i].setInterrupt(enableInterrupt);
  }
  */
}

bool CommunicationNode::sendMessage(Message transmission) {
  //Check components
  for(unsigned int i = 0; i < numberOfComponents; i++) {
    if(transmission.target == (componentList+i)->getAdress()) {
      return (componentList+i)->recieveMessage(transmission);
    }
  }

  //Check connections
  for(unsigned int i = 0; i<numberOfConnections; i++) {
    if((communicationList+i)->hasConnectionAttached(transmission.target)){
      return (communicationList+i)->sendMessage(transmission);
    }
  }
  return (communicationList)->sendMessage(transmission);

}

void CommunicationNode::loopAllAttached() {

  for(unsigned int i = 0; i<numberOfComponents; i++) {
    Message currentMessage = (componentList+i)->componentLoop();

    if(currentMessage.hasMessage == true) {
      this->sendMessage(currentMessage);
    }

  }


  for(unsigned int i = 0; i<numberOfConnections; i++) {
    String recievedMessage = (communicationList+i)->listen();

    if(!recievedMessage.equalsIgnoreCase("")) { //adress:sender:message
      //ToDo: Sepparate message

      Message currentMessage;
      int indexA;
      int indexB;
      String substring;

      indexA = recievedMessage.indexOf(';');
      substring = recievedMessage.substring(0,indexA);
      currentMessage.target = substring.toInt();

      indexB = recievedMessage.indexOf(';',indexA+1);
      substring = recievedMessage.substring(indexA+1,indexB);
      currentMessage.sender = substring.toInt();

      currentMessage.message = recievedMessage.substring(indexB+1);

      if(this->sendMessage(currentMessage)) {
        //If recieved correctly;

        currentMessage.message = "Message delivered";
        currentMessage.target = 1;
        currentMessage.sender = 1;
        this->sendMessage(currentMessage);

      }
      else {
        //If not recieved correctly

        //currentMessage.switchAdresses();
        //currentMessage.message = "error:" + currentMessage.message;

        currentMessage.message = "Error: Target = " + currentMessage.target;
        currentMessage.message = currentMessage.message + ", Sender = " + currentMessage.sender; + ", Message = " + currentMessage.message;
        currentMessage.target = 1;
        currentMessage.sender = 1;
        this->sendMessage(currentMessage);
      }
    }
  }

}
