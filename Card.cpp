#include "Card.h"

string suitToString(Suit suit) {return SuitToStringMap.at(suit);}

string rankToString(Rank rank){return rankToStringMap.at(rank);}

Card::Card(Suit suit, Rank rank) {
    s = suit;
    r = rank;
}

Suit Card::getSuit(){return s;}

Rank Card::getRank(){return r;}

string Card::toString() const {
    std::string suitSymbol = getSuitSymbol(this->s);
    return  suitSymbol + ' ' + rankToString(this->r);
}
//overlaster == operatoren for å sammenligne kort senere
bool Card::operator==(const Card& c) const {
    return (r == c.r  && s == c.s);
}

std::string Card::getSuitSymbol(Suit s){
    switch (s) {
    case Suit::clubs : return "♣";
    case Suit::diamonds : return "♦";
    case Suit::spades : return "♠";
    case Suit::hearts : return "♥";
    default: return "?";
    }
}

void Card::print(Card& c){
    std::string suitSymbol = getSuitSymbol(c.s);

    std::cout << suitSymbol  << ' ' << rankToString(c.r) << std::endl;
}

Rank StringToRank(std::string& str) {
    for (auto& ch : str) {ch = std::toupper(ch);}
    return StringToRankMap.at(str);
}

Suit CharToSuit(char& ch){
    char upperCh = std::toupper(ch);
    return CharToSuitMap.at(upperCh);
}