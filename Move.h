#include "Heaps.h"

struct MoveCommand{
    std::string fromID;
    std::string toID;
    //kortet som skal flyttes og alt under.
    Card startCard;
    bool isValid = false;

    static MoveCommand tolkInput(std::string from, std::string to, std::string cardStr);

};