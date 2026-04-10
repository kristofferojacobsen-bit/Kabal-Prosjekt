#include <GUI.h>

namespace GUI{
    //farger:
    const TDT4102::Color FELT {100 , 105 , 110}; //bordet
    static const TDT4102::Color SLOT_F {120, 125 , 130}; //tomme plasser
    static const TDT4102::Color SLOT_B {140 , 145 , 155}; // kant for tomme plasser


    //bergegne x-koordinaten for kolonne col:
    int colX(int col) { return MARGIN + col * COL_STRIDE;}

    //CardToKey
    std::string cardToKey(Card& c) {
        std::string key;
    
    //Første bokstav er fargen
        switch (c.getSuit()) {
            case Suit::spades: key = "S"; break;
            case Suit::hearts: key = "H"; break;
            case Suit::diamonds: key = "D"; break;
            case Suit::clubs: key = "C"; break;
   
    }
    std::string r = rankToString(c.getRank());
    if (r == "Ace") key += "A";
    else if (r == "King") key += "K";
    else if (r == "Queen") key += "Q";
    else if (r == "Jack") key += "J";
    else key += r;

    return key;
    }

    //CardImages load funksjon  som laster alle kortene inn i minnet
    void CardImages::load(std::string mappe) {
        const std::string suits[] = {"S", "H", "D", "C"};
        const std::string ranks[] = {"A","2","3","4","5","6","7","8","9","10","J","Q","K"};

        //lager et objekt per kort og lagri map med nøkkel

        for (auto& s : suits) {
            for (auto& r : ranks) {
                std::string key = s + r;
                std::string filNavn = mappe + key + ".png";

                faces[key] = std::make_unique<TDT4102::Image>(filNavn);
            }
        }
        back = std::make_unique<TDT4102::Image>(mappe + "back_light.png");
    }

    TDT4102::Image& CardImages::getBack() {
        return *back;
    }

    TDT4102::Image& CardImages::getImage(Card& c) {
        return *faces.at(cardToKey(c));
    }

    // ─── Tegnefunksjoner ──────────────────────────────────────────────────────────

// Tegner kortbaksiden på posisjon (x, y), skalert til CARD_W × CARD_H
void drawCardBack(AnimationWindow& win, int x, int y, CardImages& imgs) {
    win.draw_image({x, y}, imgs.getBack(), CARD_W, CARD_H);
}

// Tegner forsiden av kortet c på posisjon (x, y), skalert til CARD_W × CARD_H
void drawCardFace(AnimationWindow& win, int x, int y, Card& c, CardImages& imgs) {
    win.draw_image({x, y}, imgs.getImage(c), CARD_W, CARD_H);
}

// Tegner en tom kortplass som en farget boks med en tekstetikett i midten.
// Brukes når stock, waste, foundation eller kolonne er tom.
void drawEmptySlot(AnimationWindow& win, int x, int y, const std::string& label) {
    win.draw_rectangle({x, y}, CARD_W, CARD_H, SLOT_F, SLOT_B);
    win.draw_text({x + 4, y + CARD_H / 2 - 9}, label, Color{140, 200, 140}, 14);
}

// Tegner trekkebunken (stock) i kolonne 0 øverst.
// Viser kortbakside + antall gjenstående kort. Tom bunke vises som tom slot.
void drawStock(AnimationWindow& win, heap& s, CardImages& imgs) {
    int x = colX(0);
    win.draw_text({x, TOP_Y - 17}, "S (trekkebunke)", Color{180,220,180}, 12);
    if (s.cards.empty()) {
        drawEmptySlot(win, x, TOP_Y, "S");
    } else {
        drawCardBack(win, x, TOP_Y, imgs);
        // Vis antall kort igjen under bunken
        win.draw_text({x + 2, TOP_Y + CARD_H + 3},
                      std::to_string((int)s.cards.size()) + " kort",
                      Color{200,220,200}, 12);
    }
}

// Tegner avkastningsbunken (waste) i kolonne 1 øverst.
// Viser kun det øverste (sist trukne) kortet med forsiden opp.
void drawWaste(AnimationWindow& win, heap& w, CardImages& imgs) {
    int x = colX(1);
    win.draw_text({x, TOP_Y - 17}, "W (avkastning)", Color{180,220,180}, 12);
    if (w.cards.empty()) {
        drawEmptySlot(win, x, TOP_Y, "W");
    } else {
        drawCardFace(win, x, TOP_Y, w.cards.back(), imgs);
    }
}

// Tegner de 4 foundation-bunkene (F1–F4) i kolonnene 3–6 øverst.
// Viser øverste kort dersom bunken ikke er tom.
void drawFoundations(AnimationWindow& win, std::vector<heap>& foundations, CardImages& imgs) {
    for (int i = 0; i < 4; ++i) {
        int x = colX(3 + (int)i);
        std::string lbl = "F" + std::to_string(i + 1);
        win.draw_text({x, TOP_Y - 17}, lbl, Color{180,220,180}, 12);
        if (foundations[i].cards.empty()) {
            drawEmptySlot(win, x, TOP_Y, lbl);
        } else {
            drawCardFace(win, x, TOP_Y, foundations[i].cards.back(), imgs);
        }
    }
}

// Tegner én spillkolonne (H1–H7) med overlappende kort.
// Kort som er snudd ned (revealed=false) vises med bakside.
// Hvert kort forskyves OVERLAP piksler nedover fra forrige.
void drawColumn(AnimationWindow& win, heap& h, int col, CardImages& imgs) {
    int x = colX(col);
    std::string lbl = "H" + std::to_string(col + 1);
    win.draw_text({x, TAB_Y - 17}, lbl, Color{180,220,180}, 12);

    if (h.cards.empty()) {
        drawEmptySlot(win, x, TAB_Y, lbl);
        return;
    }

    int y = TAB_Y;
    for (int i = 0; i < h.cards.size(); ++i) {
        if (h.revealed[i])
            drawCardFace(win, x, y, h.cards[i], imgs);  // Synlig kort
        else
            drawCardBack(win, x, y, imgs);               // Skjult kort
        y += OVERLAP;  // Neste kort tegnes lenger ned
    }
}
}
