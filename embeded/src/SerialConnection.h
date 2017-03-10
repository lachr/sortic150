#ifndef SerialConnection_h
#define SerialConnection_h

#include "Modular.h"
#include "Arduino.h"

class SerialConnection : public CommunicationConnection
{
  public:
    SerialConnection(int *adressList, unsigned int numberOfAdresses, int baudRate); //(&newAttachedAdresses)[10]
    String listen();
    bool sendMessage(Message transmission);

  private:

};

#endif
