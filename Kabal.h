#include <Move.h>

class Kabal{
    private:
    sevenHeaps heaps;
    heap stock;
    heap waste;
    std::vector<heap> foundations;

    //Hjelpe funksjoner:
    heap& getHeap(std::string code);


    public:
    Kabal(CardDeck &deck) {
        deck.shuffle();
        heaps = sevenHeaps(deck);

        while (deck.cards.size() > 0){
            stock.addCard(deck.drawCard(),false);

        }
        foundations.resize(4);
    }
    void displayBoard();
    bool checkWin();
    void executeMove(std::string from, std::string to, std::string card);
    void drawFromStock();

};

