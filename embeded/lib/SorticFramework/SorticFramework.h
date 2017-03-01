#ifndef SorticFramework_h
#define SorticFramework_h

#include "Arduino.h"
#include "Modular.h"

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
    bool recieveMessage(int sender, String message);

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
    bool recieveMessage(int sender, String message);

  protected:
    bool hasPart;
    bool isMoving;
    PlacerActionType currentPlacerActionType;
    PlacerActionDirection currentPlacerActionDirection;
};

class Detector : public Component
{
  public:
    Detector(int adress, int target);
    virtual Message componentLoop();
    bool recieveMessage(int sender, String message);

  protected:
    bool scanning;
};

class SorticController : public Component
{
  public:
    //Functions;
    SorticController(int adress, int target, int mover, int placer, int detector);
    bool recieveMessage(int sender, String message);
    virtual Message componentLoop();

  protected:
    int placerAdress;
    int detectorAdress;
    int moverAdress;
    MoverPosition currentDropTarget;
    MoverPosition currentPickupTarget;
    PlacerActionDirection currentPickupDirection;
    PlacerActionDirection currentPlaceDirection;
    bool moverIsFinished;
    bool placerIsFinished;
    String currentPartMessage;
};

#endif
