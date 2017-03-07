#ifndef Modular_h
#define Modular_h

#include "Arduino.h"

enum ComponentState {
  idle,
  working,
  error,
  interrupt
};

struct Message {
    bool hasMessage = true;
    int target;
    int sender;
    String message;
};

class Component
{
  public:
    Component();
    //Component(int newAdress, int newTarget);
    virtual Message componentLoop();
    virtual bool recieveMessage(Message transmission);
    void setInterrupt(bool enableInterrupt);
    int getAdress();


  protected:
    ComponentState _componentState = idle; //ToDo: Replace _Underline
    ComponentState stateBeforeInterrupt;
    int adress;
    int target;
  };

class CommunicationConnection {
  public:
    CommunicationConnection();
    //CommunicationConnection(int adress/*(&newAttachedAdresses)[10]*/);
    virtual String listen();
    virtual bool sendMessage(Message transmission);
    bool hasConnectionAttached(int connection);

  private:
    int attachedAdress;//es[10];

};

class CommunicationNode {
  public:
    //CommunicationNode();
    CommunicationNode(Component *theComponent, CommunicationConnection *theCommunicationConnection);
    bool sendMessage(Message transmission);
    void setInterrput(bool enableInterrupt);
    void loopAllAttached();

    void attachComponent(Component *theComponent);

  private:
    Component *attachedComponent;
    CommunicationConnection *attachedConnection;
    /*
    Component attachedComponents[10];
    CommunicationConnection attachedConnections[5]; //id 0 = always top connection
    */

};

#endif
