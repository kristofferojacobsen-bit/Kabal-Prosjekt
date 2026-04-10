#include <Kabal.h>



heap& Kabal::getHeap(std::string code){
    if (code.empty()) throw std::invalid_argument("Tom heap-kode");

    char type = toupper(code[0]); //H, F, W eller S

    switch (type)
    {
    case 'W':
        return waste;

    case 'S':
        return stock;
    case 'H': {
        int index = std::stoi(code.substr(1)) - 1;
        if (index < 0 || index >= 7) throw std::invalid_argument("Ugyldig Haug-indeks");
        return heaps.heaps[index];
    }
    case 'F':{
        int index = std::stoi(code.substr(1)) - 1;
        if (index < 0 || index >= 4) throw std::invalid_argument("Ugyldig foundation-indeks");
        return foundations[index];
    }

    default:
        throw std::invalid_argument("Ukjent heap type: " + code);
    }
}

heap& Kabal::heapFraID(const std::string& code) {
    return getHeap(code);
}

void Kabal::executeMove(std::string from, std::string to, std::string card){
    
    try{
        MoveCommand cmd = MoveCommand::tolkInput(from, to, card);

        if (!cmd.isValid) {
            std::cout << "Ugyldig kortformat 'Farge''Verdi' f.eks H2 eller SA";
        }

        heap& fraHaug = getHeap(cmd.fromID);
        heap& tilHaug = getHeap(cmd.toID);

        bool isFromWaste = (toupper(cmd.fromID[0]) =='W');
        if (isFromWaste) {
            if (!(fraHaug.cards.back() == cmd.startCard)) {
                std::cout << "Ulovlig, kan kun flytte øverste kortet fra trekkebunken" << std::endl;
                return;
            }
        }

        bool isToFoundation = (cmd.toID[0] == 'F' || cmd.toID[0] == 'f');

        if (tilHaug.legalMove(cmd.startCard, tilHaug, !isToFoundation)) {
            heap::moveMultipleCards(fraHaug, tilHaug, cmd.startCard);
            std::cout << "Trekk utført !" << std::endl;
        } else {
            std::cout << "Ulovlig trekk !" << std::endl;
        }
    }catch (const std::out_of_range& e) {
        std::cout << "Feil: Fant ikke bunken eller kortet du skrev inn" << std::endl;
    
    } catch ( const std::exception& e) {
        std::cout << "Feil: " << e.what() << std::endl;
    }
}


void Kabal::displayBoard(){
    std::cout << "\n==========================================================\n";
    std::cout << "                      KABAL - STATUS                      \n";
    std::cout << "==========================================================\n";

    std::cout << "WASTE: ";
    if (waste.cards.empty()) {
        std::cout << "[Tom]";
    } else {
        // Vi bruker 0 som ID for waste i denne visningen
        waste.printHeap(0); 
    }
    std::cout << "\n----------------------------------------------------------\n";

    // 2. Vis FOUNDATIONS (Øverst til høyre - de 4 bunkene du samler i)
    std::cout << "  FOUNDATIONS (A -> K):\n";
    for (size_t i = 0; i < foundations.size(); ++i) {
        std::cout << "  F" << (i + 1) << " "; // F1, F2, F3, F4
        foundations[i].printHeap(i + 1);
    }
    std::cout << "----------------------------------------------------------\n";

    // 3. Vis TABLEAUS (De 7 kolonnene - selve spillefeltet)
    std::cout << "  TABLEAUS:\n";
    for (size_t i = 0; i < 7; ++i) {
        // Vi kaller printHeap på hver av de 7 bunkene i sevenHeaps-objektet
        std::cout << "  H" << (i + 1) << " "; 
        heaps.heaps[i].printHeap(i + 1);
    }
    std::cout << "==========================================================\n";
}

bool Kabal::checkWin(){
    // A solved game means all 52 cards are in foundations.
    // Keep this aligned with the current move logic and board state.
    int foundationCardCount = 0;
    for (heap& f : foundations) {
        if (f.cards.size() != 13) {
            return false;
        }
        if (f.cards.back().getRank() != Rank::king) {
            return false;
        }
        foundationCardCount += static_cast<int>(f.cards.size());
    }

    if (!stock.cards.empty() || !waste.cards.empty()) {
        return false;
    }

    for (heap& h : heaps.heaps) {
        if (!h.cards.empty()) {
            return false;
        }
    }

    return foundationCardCount == 52;
}

void Kabal::drawFromStock(){
    if (!stock.cards.empty()){
        Card c = stock.cards.back();
        stock.cards.pop_back();
        stock.revealed.pop_back();

        waste.addCard(c,true); // legger til synlig kort, kan ikke bruke .removecard her fordi det automatisk gjør kortet i trekkebunken synliig,
        std::cout << "Trakk " << c.toString() << " fra stock " << std::endl;
    }
    //må snu kortene om det ikke er flere i trekke bunken, og passe på at de ikke er synlige
    else if (!waste.cards.empty()){
        while(!waste.cards.empty()) {
            Card c = waste.cards.back();
            waste.cards.pop_back();
            waste.revealed.pop_back();

            stock.addCard(c,false);
        }
    } else {
        std::cout << "Ingen flere kort igjen! " << std::endl;
    }
}