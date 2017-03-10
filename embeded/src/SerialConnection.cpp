#include "Arduino.h"
#include "Modular.h"
#include "SerialConnection.h"

SerialConnection::SerialConnection(int *adressList, unsigned int numberOfAdresses, int baudRate) : CommunicationConnection(adressList, numberOfAdresses) {//(&newAttachedAdresses)[10]
  Serial.begin(baudRate);
  //Serial.println("created");
  //while(!Serial);
}

bool SerialConnection::sendMessage(Message transmission) {
  String a = "";
  a = a + transmission.target;
  a = a + ";";
  a = a + transmission.sender;
  a = a + ";";
  a = a + transmission.message;
  Serial.println(a);
  Serial.flush();

  return true;
}

String SerialConnection::listen() {
  unsigned long prevRecieveTime;
  String RecievedString = "";
  while (Serial.available())
  {
    //get input character
    char recievedCharacter = Serial.read();
    prevRecieveTime = millis();

    //Build String and return when done
    if ((recievedCharacter != 10)and(recievedCharacter != 13)) // \n = Zeilenumbruch
    {
      //Serial.println("rec");
      RecievedString += char(recievedCharacter);

    }
    else if(millis()-prevRecieveTime>100) {
      Serial.println("timeout");
      return "";

    }
    else  //String complete
    {
      break;
    }
    delay(2);
  }
  //return string if it hasn't been completed after 500ms
  if(!RecievedString.equalsIgnoreCase("")) Serial.println(RecievedString);
  return RecievedString;
}
