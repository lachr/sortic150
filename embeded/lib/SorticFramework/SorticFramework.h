#ifndef SorticFramework_h
#define SorticFramework_h

#include "Arduino.h"
#include "Modular.h"

class Detector : public Component
{
  public:
    Detector(int adress, int target);
    virtual Message componentLoop();
    bool recieveMessage(Message transmission);

  protected:
    bool scanning;
};

enum class MoverPosition {
  pickUp,
  dropA,
  dropB,
  dropC
};

class Mover : public Component
{
  public:
    Mover(int adress, int target);
    virtual Message componentLoop();
    bool recieveMessage(Message transmission);

  protected:
    bool movementComplete;
    MoverPosition currentTarget;
};

enum class MachineLogicState {
  idle,
  sorting
};

enum class PlacerActionType {
  none,
  pickUp,
  place
};

enum class PlacerActionDirection {
  left,
  right,
  front
};

class Placer : public Component
{
  public:
    Placer(int adress, int target);
    virtual Message componentLoop(); //true = complete, false = in
    bool recieveMessage(Message transmission);

  protected:
    bool hasPart;
    bool isMoving;
    PlacerActionType currentPlacerActionType;
    PlacerActionDirection currentPlacerActionDirection;
};

class SorticController : public Component
{
  public:
    //Functions;
    SorticController(int adress, int target, int mover, int placer, int detector, int sortingList);
    bool recieveMessage(Message transmission);
    Message componentLoop();

  protected:
      int placerAdress;
      int detectorAdress;
      int moverAdress;
      int sortingListAdress;

      String currentPlacePosition;
      String currentPickupPosition;
      String currentPickupDirection;
      String currentPlaceDirection;

      bool moverIsFinished;
      bool placerIsFinished;
      String currentPartMessage;
      bool setNextIdle = false;
      bool partDetected = false;
      bool partIdentified;
      int step = 1; //1 = enable scan, 2 = drive to pickup, 3 = pickup part, 4 = drive to drop point, 5 = place part, 6 = drive back, decide what to do next
};

#endif
