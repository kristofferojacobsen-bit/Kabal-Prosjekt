#include <CardDeck.h>
#pragma once

struct heap{
    std::vector<bool> revealed;
    std::vector<Card> cards;

    bool legalMove(Card c, const heap &h, bool isSevenHeaps);
    void addCard(Card c, bool visible);
    static void moveMultipleCards(heap &fromHeap,heap &toHeap,Card startCard);
    Card removeCard();
    void printHeap(int heapNum) const;
};



class sevenHeaps{
    public:
    std::vector<heap> heaps;

    // ber eksplisitt om en default konstruktør for å kunne konstruere uten å ta med deck
    sevenHeaps() = default;
    sevenHeaps(CardDeck &deck);
};