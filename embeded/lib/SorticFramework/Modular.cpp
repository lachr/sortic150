#include "Modular.h"

//some code if needed

//Component::Component(int newAdress, int newTarget) {
Component::Component() {
  //adress = newAdress;
  //target = newTarget;
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
CommunicationConnection::CommunicationConnection() { //(&newAttachedAdresses)[10]
 //attachedAdress = adress;
  /*
  for(unsigned int i = 0 ; i<sizeof(attachedAdresses)/sizeof(attachedAdresses[0]); i++) {
    attachedAdresses[i] = newAttachedAdresses[i];
  }
  */
}

bool CommunicationConnection::hasConnectionAttached(int connection){
  /*
  for(unsigned int i = 0 ; i<sizeof(attachedAdresses)/sizeof(attachedAdresses[0]); i++) {
    if(attachedAdresses[i]==connection) {
      return true;
    }
  }
  */
  if(attachedAdress == connection) return true;
  return false;

}

//CommunicationNode::CommunicationNode() {

CommunicationNode::CommunicationNode(Component *theComponent, CommunicationConnection *theCommunicationConnection) {
  attachedComponent = theComponent;
  attachedConnection = theCommunicationConnection;

}


void CommunicationNode::setInterrput(bool enableInterrupt) {
  /*
  for(unsigned int i = 0; i < sizeof(attachedComponents)/sizeof(attachedComponents[0]); i++) {
    attachedComponents[i].setInterrupt(enableInterrupt);
  }
  */
}

bool CommunicationNode::sendMessage(Message transmission) {
  if(transmission.target == attachedComponent->getAdress()) {
    return attachedComponent->recieveMessage(transmission);
  }
  else {
    return attachedConnection->sendMessage(transmission);
  }

  /*
  for(unsigned int i = 0; i < sizeof(attachedComponents)/sizeof(attachedComponents[0]); i++) {
    if(target == attachedComponents[i].getAdress()) {
      return attachedComponents[i].recieveMessage(sender, message);
    }
  }

  for(unsigned int i = 0; i<sizeof(attachedConnections)/sizeof(attachedConnections[0]); i++) {
    if(target == attachedConnections[i].hasConnectionAttached(target)){
      return attachedConnections[i].sendMessage(target, sender, message);
    }
  }
  return attachedConnections[0].sendMessage(target, sender, message);
  */
}

void CommunicationNode::loopAllAttached() {
  Message currentMessage = attachedComponent->componentLoop();
  if(currentMessage.hasMessage == true) {
    this->sendMessage(currentMessage);
  }

  String recievedMessage = attachedConnection->listen();

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

    if(currentMessage.target == 2) {

      bool hasWorked = attachedComponent->recieveMessage(currentMessage);
      if(!hasWorked){
        Message errorMessage;
        errorMessage.target = 1;
        errorMessage.sender = 3;
        errorMessage.message = "message not accepted";

        attachedConnection->sendMessage(errorMessage);
      }
    }
    //attachedConnection->sendMessage(target, sender, message);
  }

  /*
  for(unsigned int i = 0; i < sizeof(attachedComponents)/sizeof(attachedComponents[0]); i++) {
    Message currentMessage = attachedComponents[i].componentLoop();
    if(currentMessage.hasMessage) {
      this->sendMessage(currentMessage.target, currentMessage.sender, currentMessage.message);
    }

  }
  for(unsigned int i = 0; i < sizeof(attachedConnections)/sizeof(attachedConnections[0]); i++) {
    String recievedMessage = attachedConnections[i].listen();
    if(!recievedMessage.equalsIgnoreCase("")) {
      //ToDo: Sepparate message
    }
  }
  */
}
