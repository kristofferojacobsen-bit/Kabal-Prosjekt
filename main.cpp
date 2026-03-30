//


#include "std_lib_facilities.h"
#include <Kabal.h>

//------------------------------------------------------------------------------'


int main() {
    CardDeck deck;
    Kabal spill(deck);
    std::string linje;
    bool ferdig = false;

    std::cout << "Velkommen til kabal ! (Skriv 'slutt' for å avslutte eller 'trekk' for å trekke et nytt kort)" << std::endl;
    std::cout << "Eksempel input er 'H1 H2 CA' her flyttet jeg kløver ess fra første haug til andre" << std::endl;
    std::cout << "Bruk J,Q,K,A for bilde kortene" << std::endl;

    while (!ferdig) {
        spill.displayBoard();

        std::cout << std::endl;

        std::getline(std::cin,linje);
        if (linje == "slutt") break;

        std::stringstream ss(linje);
        std::string cmd,fra,til,kort;
        ss >> cmd;

        if (cmd == "trekk") {
            spill.drawFromStock();
        }
        else {
            //antar at siden det ikke var trekk at resten av stringen er fra til kort
            fra = cmd;
            ss >> til >> kort;

            if(!til.empty() && !kort.empty()) {
                spill.executeMove(fra,til,kort);
            } else {
                std::cout << "Ugyldig format! skriv kun 'fra', 'til', 'kort' eller draw" << std::endl;
            }
        }

        ferdig = spill.checkWin();

       
    }
    if (ferdig) {
        std::cout << "Gratulerer, du har vunnet!" << std::endl;
    }
    return 0;
}

//------------------------------------------------------------------------------
