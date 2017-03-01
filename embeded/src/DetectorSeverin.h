#ifndef DetectorSeverin_h
#define DetectorSeverin_h

#include "Arduino.h"
#include "SorticFramework.h"
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

class DetectorSeverin : public Detector
{
  public:
    DetectorSeverin(int adress, int target, MFRC522 *tempPartDetector);
    Message componentLoop();
  private:
    MFRC522 PartDetector;
};

#endif
