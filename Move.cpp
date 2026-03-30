#include "Move.h"

MoveCommand MoveCommand::tolkInput (std::string from,std::string to, std::string cardStr){
    //opprette et move
    MoveCommand cmd;
    cmd.fromID = from;
    cmd.toID = to;



    try {
   //sjekker at strengen er lang nok
   if (cardStr.length() < 2) throw std::invalid_argument("For kort streng");

   char suitChar = cardStr[0];
   std::string rankStr = cardStr.substr(1);

   //bruker hjelpefunksjoner
   Suit s = CharToSuit(suitChar);
   Rank r = StringToRank(rankStr);

   cmd.startCard = Card(s,r);
   cmd.isValid = true;
    } catch (...) {
        std::cout << "Feil: ugyldig kortformat (" << cardStr << ")" << std::endl;
        cmd.isValid = false;
    }

    return cmd;
}
