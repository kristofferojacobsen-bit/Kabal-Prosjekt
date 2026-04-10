#include <Klikk.h>

class KabalWin : public TDT4102::AnimationWindow{
    private:
    CardDeck deck;
    Kabal spill;
    GUI::CardImages images;


    bool prevMouseDown = false; 

    // Husker Klikk 1 slik at vi kan bruke det i Klikk 2
    bool harValgtKort = false;
    std::string fraHaugID = "";        // F.eks. "T3" eller "W"
    std::string valgtKortStreng = "";  // F.eks. "H10" eller "CA"
    int valgtKortIndex = -1;

    // Brukeropplevelse
    std::string melding = "";          // Tekst som vises på skjermen
    bool ferdig = false;
    void tegnMarkering();
    


    public:
    KabalWin();
    void handterKlikk();
    bool isFinished() const { return ferdig; }

    void draw();
};