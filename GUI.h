#include <Kabal.h>
#include <AnimationWindow.h>
#include <widgets/TextInput.h>
#include <widgets/Button.h>
#include <memory>

namespace GUI {
    // Layout konstater, for å plassere ting på skjermen og bestemme størrelser

    extern const TDT4102::Color FELT;

    static const int WIN_W      = 1250;  // Bredde på vinduet i piksler
    static const int WIN_H      = 790;   // Høyde på vinduet i piksler
    static const int CARD_W     = 90;    // Kortbredde i piksler
    static const int CARD_H     = 130;   // Korthøyde i piksler
    static const int COL_STRIDE = 165;   // Horisontal avstand mellom kolonner
    static const int MARGIN     = 28;    // Venstre marg fra kanten av vinduet
    static const int TOP_Y      = 35;    // Y-posisjon for øverste rad (stock, waste, foundations)
    static const int TAB_Y      = 190;   // Y-posisjon for start av de 7 kolonnene
    static const int OVERLAP    = 25;    // Hvor mange piksler hvert kort stikker ned forbi kortet over

    //Lager en struct cardImages for å holde alle 53 bildene som er lastet inn fra disk

    struct CardImages {
        std::map<std::string, std::unique_ptr<TDT4102::Image>> faces;
        std::unique_ptr<TDT4102::Image> back;

        void load(std::string mappe = "playingCardsImages/");

        TDT4102::Image& getImage(Card& c);

        TDT4102::Image& getBack();
    };

    int colX(int col);

    //tegnefunksjoner


    std::string cardToKey(Card& c);
// Tegner baksiden av et kort på posisjon (x, y)
void drawCardBack(TDT4102::AnimationWindow& win, int x, int y, CardImages& imgs);

// Tegner forsiden av kortet c på posisjon (x, y)
void drawCardFace(TDT4102::AnimationWindow& win, int x, int y, const Card& c, CardImages& imgs);

// Tegner en tom kortplass med en tekstetikett (f.eks. "F1", "H3")
void drawEmptySlot(TDT4102::AnimationWindow& win, int x, int y, const std::string& label);

// Tegner trekkebunken (stock) øverst til venstre
void drawStock(TDT4102::AnimationWindow& win, heap& s, CardImages& imgs);

// Tegner avkastningsbunken (waste) – viser øverste kort
void drawWaste(TDT4102::AnimationWindow& win, heap& w, CardImages& imgs);

// Tegner de 4 foundation-bunkene (samlingsplassene) øverst til høyre
void drawFoundations(TDT4102::AnimationWindow& win, std::vector<heap>& foundations, CardImages& imgs);

// Tegner én av de 7 spillkolonnene med overlappende kort
void drawColumn(TDT4102::AnimationWindow& win, heap& h, int col, CardImages& imgs);

// Tegner hele brettet: kaller alle drawX-funksjonene over
void drawBoard(TDT4102::AnimationWindow& win, Kabal& game, CardImages& imgs);

}

