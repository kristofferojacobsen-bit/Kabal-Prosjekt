#pragma once
#include "std_lib_facilities.h"


enum class Suit {clubs, diamonds, spades, hearts};
enum class Rank {ace = 1, two, three, four, five, six, seven, eight, nine, ten, jack, queen, king = 13};

const map<Suit, string> SuitToStringMap {
	{Suit::clubs, "Clubs"},
	{Suit::diamonds, "Diamonds"},
	{Suit::hearts, "Hearts"},
	{Suit::spades, "Spades"}
};

string suitToString(Suit suit);

const map<Rank, string> rankToStringMap {
	{Rank::ace, "Ace"},
	{Rank::two, "2"},
	{Rank::three, "3"},
	{Rank::four, "4"},
	{Rank::five, "5"},
	{Rank::six, "6"},
	{Rank::seven, "7"},
	{Rank::eight, "8"},
	{Rank::nine, "9"},
	{Rank::ten, "10"},
	{Rank::jack, "Jack"},
	{Rank::queen, "Queen"},
	{Rank::king, "King"},
};

const std::map<char,Suit> CharToSuitMap{
	{'H',Suit::hearts},
	{'C',Suit::clubs},
	{'S',Suit::spades},
	{'D',Suit::diamonds},
};

Suit CharToSuit(char& ch);

Rank StringToRank(std::string& str) ;
 
const std::map<std::string,Rank> StringToRankMap {
	{"A",Rank::ace},
	{"K",Rank::king},
	{"Q",Rank::queen},
	{"J",Rank::jack},
	{"10",Rank::ten},
	{"9",Rank::nine},
	{"8",Rank::eight},
	{"7",Rank::seven},
	{"6",Rank::six},
	{"5",Rank::five},
	{"4",Rank::four},
	{"3",Rank::three},
	{"2",Rank::two},
};

string rankToString(Rank rank);

class Card{
private:
	Suit s;
	Rank r;

public:
	Suit getSuit() const;
	static std::string getSuitSymbol(Suit s);
	Rank getRank() const;
	std::string toString() const;
	static void print(Card& c);
	Card() = default;
	Card(Suit suit, Rank rank);
	bool operator==(const Card& a) const;
};