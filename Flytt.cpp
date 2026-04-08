#include "Flytt.h"
#include <sstream>

// Flytt.cpp – Implementasjon av klikk-basert kortflytting.
// Logikken følger et to-klikks-system:
//   1. Klikk på et synlig kort → kortet velges og markeres med gul ramme
//   2. Klikk på destinasjon  → trekket utføres via spillogikken i Kabal

using namespace TDT4102;

namespace Flytt {

// ─── byggSoner ────────────────────────────────────────────────────────────────
// Oppretter en liste over alle klikkbare rektangler på brettet.
// Kalles hvert frame slik at sonene alltid er i sync med bretttilstanden.
std::vector<Sone> byggSoner(Kabal& game) {
    std::vector<Sone> soner;

    // Stock (trekkebunke) – alltid klikkbar, klikk trekker ett kort
    {
        int x = GUI::colX(0);
        soner.push_back({x, GUI::TOP_Y, GUI::CARD_W, GUI::CARD_H, "S", -1});
    }

    // Waste (avkastning) – kun øverste kort er klikkbart (kan flyttes derfra)
    {
        int x = GUI::colX(1);
        heap& w = game.getWaste();
        if (!w.cards.empty()) {
            soner.push_back({x, GUI::TOP_Y, GUI::CARD_W, GUI::CARD_H,
                             "W", (int)w.cards.size() - 1});
        }
    }

    // Foundations F1–F4 – alltid klikkbare som destinasjon
    // kortIndex = -1 hvis tom, ellers indeks til øverste kort
    for (int i = 0; i < 4; ++i) {
        int x = GUI::colX(3 + i);
        std::string id = "F" + std::to_string(i + 1);
        heap& f = game.getFoundations()[i];
        soner.push_back({x, GUI::TOP_Y, GUI::CARD_W, GUI::CARD_H,
                         id, f.cards.empty() ? -1 : (int)f.cards.size() - 1});
    }

    // Heaps H1–H7 – hvert kort i kolonnen er en egen sone
    for (int col = 0; col < 7; ++col) {
        int x = GUI::colX(col);
        std::string id = "H" + std::to_string(col + 1);
        heap& h = game.getHeaps().heaps[col];

        if (h.cards.empty()) {
            // Tom kolonne: én sone for hele plassen (kan motta en konge)
            soner.push_back({x, GUI::TAB_Y, GUI::CARD_W, GUI::CARD_H, id, -1});
        } else {
            for (int i = 0; i < (int)h.cards.size(); ++i) {
                int y = GUI::TAB_Y + i * GUI::OVERLAP;

                // Alle kort unntatt det siste har klikkbar høyde = OVERLAP (den synlige stripen).
                // Det siste kortet er fullt synlig og får full CARD_H som klikkbar høyde.
                int cardH = (i == (int)h.cards.size() - 1) ? GUI::CARD_H : GUI::OVERLAP;

                soner.push_back({x, y, GUI::CARD_W, cardH, id, i});
            }
        }
    }

    return soner;
}

// ─── treffSone ────────────────────────────────────────────────────────────────
// Returnerer true dersom musekoordinatene (mx, my) er innenfor sonen.
bool treffSone(const Sone& sone, int mx, int my) {
    return mx >= sone.x && mx < sone.x + sone.w &&
           my >= sone.y && my < sone.y + sone.h;
}

// ─── finnKlikketSone ──────────────────────────────────────────────────────────
// Søker bakfra i listen (siste = øverst visuelt) og returnerer den første sonen
// som inneholder punktet (mx, my). Slik får overlappende kort riktig prioritet.
const Sone* finnKlikketSone(const std::vector<Sone>& soner, int mx, int my) {
    for (int i = (int)soner.size() - 1; i >= 0; --i) {
        if (treffSone(soner[i], mx, my)) {
            return &soner[i];
        }
    }
    return nullptr; // Ingenting ble truffet
}

// ─── haandterKlikk ────────────────────────────────────────────────────────────
// Hoved-logikken for klikk-basert kortflytting.
// Kalles én gang per museknapp-nedtrykk (edge-detection håndteres i main.cpp).
void haandterKlikk(Kabal& game, FlyttTilstand& tilstand,
                   const std::vector<Sone>& soner, int mx, int my) {

    const Sone* sone = finnKlikketSone(soner, mx, my);

    // ── Klikk utenfor brettet: avbryt eventuelt pågående valg ──
    if (!sone) {
        if (tilstand.harValgt) {
            tilstand.harValgt = false;
            tilstand.melding = "Valg avbrutt";
        }
        return;
    }

    // ── Klikk på stock: trekk ett kort fra trekkebunken ──
    if (sone->heapID == "S") {
        tilstand.harValgt = false; // Avbryt eventuelt pågående valg

        // Fang opp cout-utskrift fra spillogikken for å vise som melding
        std::ostringstream captured;
        std::streambuf* buf = std::cout.rdbuf(captured.rdbuf());
        game.drawFromStock();
        std::cout.rdbuf(buf);

        tilstand.melding = captured.str();
        // Fjern linjeskift på slutten av meldingen
        while (!tilstand.melding.empty() &&
               (tilstand.melding.back() == '\n' || tilstand.melding.back() == '\r'))
            tilstand.melding.pop_back();
        return;
    }

    // ── Første klikk: velg et kort ──
    if (!tilstand.harValgt) {
        heap& h = game.getHeap(sone->heapID);

        // Kan ikke velge fra en tom plass
        if (h.cards.empty() || sone->kortIndex < 0) {
            tilstand.melding = "Ingen kort å velge her";
            return;
        }

        // Kan ikke velge skjulte (nedvendte) kort
        if (!h.revealed[sone->kortIndex]) {
            tilstand.melding = "Kan ikke velge skjult kort";
            return;
        }

        // Lagre det valgte kortet i tilstanden
        tilstand.harValgt = true;
        tilstand.fraID = sone->heapID;
        tilstand.valgtKort = h.cards[sone->kortIndex];
        tilstand.valgtKortIndex = sone->kortIndex;
        tilstand.melding = "Valgt: " + tilstand.valgtKort.toString() +
                           " fra " + tilstand.fraID;
        return;
    }

    // ── Andre klikk på samme kort: avbryt valget ──
    if (sone->heapID == tilstand.fraID && sone->kortIndex == tilstand.valgtKortIndex) {
        tilstand.harValgt = false;
        tilstand.melding = "Valg avbrutt";
        return;
    }

    // ── Andre klikk på destinasjon: utfør trekket ──

    // Bygg kortstreng i formatet executeMove forventer, f.eks. "SA", "H10", "CK"
    char suitChar;
    switch (tilstand.valgtKort.getSuit()) {
        case Suit::spades:   suitChar = 'S'; break;
        case Suit::hearts:   suitChar = 'H'; break;
        case Suit::diamonds: suitChar = 'D'; break;
        case Suit::clubs:    suitChar = 'C'; break;
    }
    std::string rankStr = rankToString(tilstand.valgtKort.getRank());
    if      (rankStr == "Ace")   rankStr = "A";
    else if (rankStr == "Jack")  rankStr = "J";
    else if (rankStr == "Queen") rankStr = "Q";
    else if (rankStr == "King")  rankStr = "K";
    std::string kortStr = std::string(1, suitChar) + rankStr;

    // Fang opp cout-utskrift fra spillogikken for å vise som melding
    std::ostringstream captured;
    std::streambuf* buf = std::cout.rdbuf(captured.rdbuf());
    game.executeMove(tilstand.fraID, sone->heapID, kortStr);
    std::cout.rdbuf(buf);

    tilstand.melding = captured.str();
    while (!tilstand.melding.empty() &&
           (tilstand.melding.back() == '\n' || tilstand.melding.back() == '\r'))
        tilstand.melding.pop_back();

    // Nullstill valget uansett om trekket var gyldig eller ikke
    tilstand.harValgt = false;
}

// ─── tegnMarkering ────────────────────────────────────────────────────────────
// Tegner en gul ramme rundt det valgte kortet slik at spilleren ser hva som er valgt.
// For kolonner strekkes rammen ned til å dekke alle kort i stakken under det valgte.
void tegnMarkering(AnimationWindow& win, FlyttTilstand& tilstand, Kabal& game) {
    if (!tilstand.harValgt) return; // Ingenting valgt, ingenting å tegne

    // Finn piksel-posisjon til det valgte kortet basert på heap-type
    int x = 0, y = 0;

    if (tilstand.fraID == "W") {
        // Waste: alltid i kolonne 1, øverste rad
        x = GUI::colX(1);
        y = GUI::TOP_Y;
    } else if (tilstand.fraID[0] == 'F') {
        // Foundation: kolonnene 3–6, øverste rad
        int fi = std::stoi(tilstand.fraID.substr(1)) - 1;
        x = GUI::colX(3 + fi);
        y = GUI::TOP_Y;
    } else if (tilstand.fraID[0] == 'H') {
        // Kolonne: y forskyves med OVERLAP per kort
        int col = std::stoi(tilstand.fraID.substr(1)) - 1;
        x = GUI::colX(col);
        y = GUI::TAB_Y + tilstand.valgtKortIndex * GUI::OVERLAP;
    }

    // Beregn høyde på markeringsrammen.
    // For kolonner: rammen dekker det valgte kortet og alle kort under (hele stakken).
    // For andre bunker: kun ett kort → full CARD_H.
    int h = GUI::CARD_H;
    if (tilstand.fraID[0] == 'H') {
        int col = std::stoi(tilstand.fraID.substr(1)) - 1;
        heap& hp = game.getHeaps().heaps[col];
        int antallKort = (int)hp.cards.size() - tilstand.valgtKortIndex;
        h = (antallKort - 1) * GUI::OVERLAP + GUI::CARD_H;
    }

    // Tegn en 3 piksler bred gul ramme rundt det valgte kortet
    // ved å tegne tre rektangler som er 1 piksel større for hvert lag
    Color markeringsFarge{255, 220, 0};
    for (int t = 0; t < 3; ++t) {
        win.draw_rectangle({x - t, y - t}, GUI::CARD_W + 2*t, h + 2*t,
                           Color::transparent, markeringsFarge);
    }
}

} // namespace Flytt
