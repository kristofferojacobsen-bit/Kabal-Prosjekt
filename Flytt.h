#pragma once

// Flytt.h – Deklarasjoner for klikk-basert kortflytting.
// Implementerer et to-klikks-system: første klikk velger et kort,
// andre klikk velger destinasjon og utfører trekket.

#include "GUI.h"

namespace Flytt {

// ─── Sone ─────────────────────────────────────────────────────────────────────
// Representerer et klikkbart rektangel på brettet.
// Hvert synlig kort, tom kortplass og trekkebunke er én sone.
struct Sone {
    int x, y;          // Øverste venstre hjørne i piksler
    int w, h;          // Bredde og høyde i piksler
    std::string heapID; // Hvilken bunke sonen tilhører: "H1"–"H7", "W", "S", "F1"–"F4"
    int kortIndex;      // Indeks til kortet i bunken. -1 = tom plass / hele bunken
};

// ─── FlyttTilstand ────────────────────────────────────────────────────────────
// Holder den nåværende tilstanden for klikk-interaksjonen mellom frames.
// Oppdateres av haandterKlikk() og leses av tegnMarkering().
struct FlyttTilstand {
    bool harValgt = false;      // true dersom spilleren har valgt et kort og venter på destinasjon
    std::string fraID;          // Heap-kode for det valgte kortet, f.eks. "H3"
    Card valgtKort;             // Selve det valgte kort-objektet
    int valgtKortIndex = -1;    // Indeks i fra-bunken (brukes for å beregne markeringsposisjon)
    std::string melding;        // Tilbakemelding som vises på skjermen, f.eks. "Ulovlig trekk!"

    bool prevMouseDown = false; // Forrige frames museknapp-tilstand, brukes for edge-detection
                                // slik at ett klikk ikke registreres som mange
};

// Bygger og returnerer en liste over alle klikkbare soner for nåværende bretttilstand.
// Må kalles på nytt hver gang brettet endrer seg (dvs. hvert frame).
std::vector<Sone> byggSoner(Kabal& game);

// Returnerer true dersom musepunktet (mx, my) er innenfor sonen
bool treffSone(const Sone& sone, int mx, int my);

// Finner og returnerer peker til sonen som ble klikket.
// Søker bakfra i listen slik at øverste overlappende kort har prioritet.
// Returnerer nullptr dersom ingenting ble truffet.
const Sone* finnKlikketSone(const std::vector<Sone>& soner, int mx, int my);

// Håndterer ett museklikk på posisjon (mx, my).
// Oppdaterer tilstand og kaller game.executeMove() eller game.drawFromStock() ved behov.
void haandterKlikk(Kabal& game, FlyttTilstand& tilstand,
                   const std::vector<Sone>& soner, int mx, int my);

// Tegner en gul ramme rundt det valgte kortet (og kortene under det i kolonnen).
// Gjør ingenting dersom ingen kort er valgt.
void tegnMarkering(TDT4102::AnimationWindow& win, FlyttTilstand& tilstand,
                   Kabal& game);

} // namespace Flytt
