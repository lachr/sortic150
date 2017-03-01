#ifndef SerialConnection_h
#define SerialConnection_h

#include "Modular.h"
#include "Arduino.h"

class SerialConnection : public CommunicationConnection
{
  public:
    SerialConnection(int adress, int baudRate); //(&newAttachedAdresses)[10]
    String listen();
    bool sendMessage(int target, int sender, String message);

  private:

};

#endif
