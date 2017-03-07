#ifndef Modular_h
#define Modular_h

#include "Arduino.h"

enum ComponentState {
  idle,
  working,
  error,
  interrupt
};

class Message {
  public:
    bool hasMessage = false;
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
    virtual bool recieveMessage(int sender, String message);
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
    virtual bool sendMessage(int target, int sender, String message);
    bool hasConnectionAttached(int connection);

  private:
    int attachedAdress;//es[10];

};

class CommunicationNode {
  public:
    //CommunicationNode();
    CommunicationNode(Component *theComponent, CommunicationConnection *theCommunicationConnection);
    bool sendMessage(int target, int sender, String message);
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
