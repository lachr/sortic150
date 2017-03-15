#ifndef SortinList_h
#define SortinList_h

#include "Arduino.h"
#include "Modular.h"

class SortingList : public Component {
  public:
    SortingList(int adress, int target, String part1, String pat2);
    Message componentLoop();
    bool recieveMessage(Message transmission);

  private:
    String part1;
    String part2;

    bool messageSent;
    Message nextMessage;
};

#endif
