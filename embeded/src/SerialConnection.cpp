#include "Arduino.h"
#include "Modular.h"
#include "SerialConnection.h"

SerialConnection::SerialConnection(int adress, int baudRate) : CommunicationConnection() { //(&newAttachedAdresses)[10]
  //this->attachedAdress = adress;
  Serial.begin(baudRate);
}

bool SerialConnection::sendMessage(Message transmission) {
  Serial.print(transmission.target);
  Serial.print(";");
  Serial.print(transmission.sender);
  Serial.print(";");
  Serial.println(transmission.message);
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
  //if(!RecievedString.equals("")) Serial.println(RecievedString);
  if(!RecievedString.equalsIgnoreCase("")) Serial.println(RecievedString);
  return RecievedString;
  /*
  if(!RecievedString.equals("")) {
    String TempString = RecievedString;
    RecievedString = "";
    return(TempString);
  }

  //return empty, if no string was read
  //Serial.println("error");
  return("");
  */
}
