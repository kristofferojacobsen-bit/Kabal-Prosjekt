#include "CardDeck.h"

// konstruktør
CardDeck::CardDeck(){
    for (int s = 0; s < 4; s++){
        for (int r = 1; r <= 13; r++){
            cards.push_back(
                Card(static_cast<Suit>(s),
                    static_cast<Rank>(r)));
        }
    }
}


// Bytte to enkelt kort
void CardDeck::swap(int c1, int c2){
    Card temp = cards.at(c1);
    cards.at(c1) = cards.at(c2);
    cards.at(c2) = temp;

}


// skrive ut et kort
void CardDeck::print(){
    for (Card& card : cards){
        string str = card.toString();
        cout << str << endl;
    }
}


// stokke kortene
void CardDeck::shuffle(){
    random_device rd;
	default_random_engine generator(rd());
    std::shuffle(cards.begin(), cards.end(), generator);
}


//trekke
Card CardDeck::drawCard(){
    Card kortet = cards.back();
    cards.pop_back();
    return kortet;
}
