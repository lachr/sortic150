#ifndef DummyComponents_h
#define DummyComponents_h

#include "Arduino.h"
#include "SorticFramework.h"
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

class DummyDetector : public Detector
{
  public:
    DummyDetector(int adress, int target);
    Message componentLoop();
  private:

};

#endif
