#include <Heaps.h>

sevenHeaps::sevenHeaps(CardDeck &deck){
    //heaps trenger størrelse for å kunne push_back
    heaps.resize(7);
    for (size_t i = 0; i < 7; ++i){
        for (size_t antall_kort = 0; antall_kort <= i; ++antall_kort){
            //legger til kort fra kortstokken til kortene
            heaps[i].cards.push_back(deck.drawCard());
            // gjør kun siste kortet synlig
            heaps[i].revealed.push_back(antall_kort == i);
        }
    }
}



void heap::addCard(Card c, bool visible){
    //legger til et kort øverst og legger til en visible
    cards.push_back(c);
    revealed.push_back(visible);
}

Card heap::removeCard(){
    //sjekker at heapen ikke er tom
    if (this->cards.empty()) {return Card();}
    //fjerner kortet 
    Card card = cards.back();
    cards.pop_back();

    // snu det som nå er siste
    revealed.pop_back();
    if (!revealed.empty()){revealed.back() = true;}
    return card;
}

bool heap::legalMove(Card c, const heap& h, bool isSevenHeaps){
    Rank rank = c.getRank();
    int suit = static_cast<int>(c.getSuit());
    //må sjekke om heapen er tom før vi henter ut kort, kan like så greit sjekke om det er gyldig trekk samtidig for tomme heaps
    if (h.cards.empty()) {
        if (isSevenHeaps){
            if (rank == Rank::king){return true;}
        }
        else{
            if (rank == Rank::ace) {return true;}
        }
        return false;
    }

    Card cardDest = h.cards.back();
    int rankDest = static_cast<int>(cardDest.getRank());
    int suitDest = static_cast<int>(cardDest.getSuit());

    if (isSevenHeaps){
        //sjekker om de er sammme farge, sjekk definisjonen av suit, partall er svarte, oddetall er rød
        //sjekker så om forskjellen mellom kortene er nøyaktig 1
        if (suitDest % 2 != suit % 2){
        if (rankDest - static_cast<int>(rank) == 1) {return true;}
        }  
    }
    //sjekker om fargene er like her som passer med regelen for sort-samlingene
    else if (!isSevenHeaps){
        if (suitDest == suit){
            if (static_cast<int>(rank) - rankDest == 1) {return true;}
        }
    }
        return false;
}


void heap::moveMultipleCards(heap& fromHeap,heap& toHeap,Card startCard){
    int index = -1;
    for (int i = 0; i < static_cast<int>(fromHeap.cards.size()); ++i){if (fromHeap.cards[i] == startCard) {index = i; break;}}
    if (index == -1) {return;}

    //flytter og så fjerner kortene, passer på å ta med revealed status og
    toHeap.cards.insert(toHeap.cards.end(),fromHeap.cards.begin() + index ,fromHeap.cards.end());
    toHeap.revealed.insert(toHeap.revealed.end(),fromHeap.revealed.begin() + index,fromHeap.revealed.end());

    fromHeap.cards.erase(fromHeap.cards.begin() + index ,fromHeap.cards.end());
    fromHeap.revealed.erase(fromHeap.revealed.begin() + index ,fromHeap.revealed.end());
    //snu siste kortet om det ikke er snudd fra før
    if (!fromHeap.cards.empty()) {fromHeap.revealed.back() = true;}
}



void heap::printHeap(int heapNum) const {
    if (this->cards.empty()) {
        std::cout << " : " << std::endl;
        return;
    }

    int numHidden = 0;
    std::string result = " : ";
    // teller skjulte kort og om det er noen, legger til i resultat string

    for (bool visible : this->revealed){
        if (!visible) {++numHidden;} 
    }
    if (numHidden != 0) {result += std::to_string(numHidden) + " XX ";}
    for (size_t i = 0; i < this->cards.size() && i < this->revealed.size(); ++i){
        if (this->revealed[i]) {result += this->cards[i].toString() + " ";}
    }
    cout << result << endl;
}
