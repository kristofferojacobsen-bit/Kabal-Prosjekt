#pragma once
#include "Card.h"

// definerer klassen CardDeck med medlemssvvariabel cards som er en vektor kort, i tillegg til flere funksjoner som trengs
class CardDeck{
private:
    void swap(int c1, int c2);
public:
    std::vector<Card> cards;
    void print();
    void shuffle();
    Card drawCard();
    CardDeck();

};



