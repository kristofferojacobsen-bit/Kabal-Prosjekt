// main.cpp – Inngangspunkt for kabal-spillet.
// Setter opp vindu, laster kortbilder, oppretter widgets og kjører spilløkken.
// Støtter både tastaturbasert input (tekstfelt) og klikk-basert kortflytting.

#include "std_lib_facilities.h"
#include "GUI.h"
#include "Flytt.h"
#include <sstream>

using namespace TDT4102;

int main() {
    // Opprett kortstokk og kabal-spillobjekt (kortstokken blandes i konstruktøren)
    CardDeck deck;
    Kabal spill(deck);

    // Opprett spillvinduet
    AnimationWindow win(50, 50, GUI::WIN_W, GUI::WIN_H, "Kabal");
    win.setBackgroundColor(Color{34, 120, 50}); // Grønn bordfarve

    // Last inn alle 53 kortbilder fra Kort-mappen én gang før spilløkken.
    // Bilder lastes utenfor løkken for å unngå å lese fra disk hvert frame.
    GUI::CardImages kortBilder;
    kortBilder.load("/Users/kristoffereugenodegardjacobsen/Desktop/Kabal/Kort");
/*
    // ── Widgets ──────────────────────────────────────────────────────────────
    // Tekstfelt for å skrive kommandoer, f.eks. "H1 H2 CA" eller "trekk"
    TextInput inputFelt({GUI::MARGIN, GUI::INPUT_Y}, 430, 38, "");
    win.add(inputFelt);

    // Knapp for å utføre kommandoen i tekstfeltet
    Button utforBtn({GUI::MARGIN + 440, GUI::INPUT_Y}, 110, 38, "Utfor");
    // Knapp for å trekke ett kort fra trekkebunken (shortcut for "trekk"-kommando)
    Button trekBtn ({GUI::MARGIN + 560, GUI::INPUT_Y}, 110, 38, "Trekk"); 

    std::string melding;       // Tilbakemelding som vises på skjermen
    bool ferdig  = false;      // Settes til true når spillet er vunnet
    bool prevEnter = false;    // Forrige frames enter-tilstand (for edge-detection)
    */

    // Tilstand for klikk-basert kortflytting (se Flytt.h)
    Flytt::FlyttTilstand flyttTilstand;

    // ── Prosesseringsfunksjon for tekstkommandoer ─────────────────────────────
    // Tar imot en kommandostreng, parser den og kaller riktig spillfunksjon.
    // Fanger opp cout-utskrift fra spillogikken og lagrer den som melding.
    auto prosesser = [&](const std::string& linje) {
        if (linje == "slutt") { win.close(); return; } // Avslutt spillet
        if (linje.empty())    return;                  // Ignorer tomme kommandoer

        // Omdiriger cout slik at spillogiikkens meldinger fanges opp
        std::ostringstream captured;
        std::streambuf* buf = std::cout.rdbuf(captured.rdbuf());

        std::stringstream ss(linje);
        std::string cmd, fra, til, kort;
        ss >> cmd;

        if (cmd == "trekk") {
            // Trekk ett kort fra trekkebunken
            spill.drawFromStock();
        } else {
            // Flytt-kommando: format "FRA TIL KORT", f.eks. "H1 H2 CA"
            fra = cmd;
            ss >> til >> kort;
            if (!til.empty() && !kort.empty())
                spill.executeMove(fra, til, kort);
            else
                std::cout << "Ugyldig format! Eks: H1 H2 CA";
        }

        // Gjenopprett cout og hent fanget melding
        std::cout.rdbuf(buf);
        melding = captured.str();
        // Fjern linjeskift på slutten
        while (!melding.empty() &&
               (melding.back() == '\n' || melding.back() == '\r'))
            melding.pop_back();

        inputFelt.setText(""); // Tøm tekstfeltet etter utføring
        ferdig = spill.checkWin();
    };

    // Knytt knapp-callbacks til prosesseringsfunksjonen
    utforBtn.setCallback([&]() { prosesser(inputFelt.getText()); });

    // Trekk-knappen trekker direkte fra stock uten å gå via tekstfeltet
    trekBtn.setCallback([&]() {
        std::ostringstream captured;
        std::streambuf* buf = std::cout.rdbuf(captured.rdbuf());
        spill.drawFromStock();
        std::cout.rdbuf(buf);
        melding = captured.str();
        while (!melding.empty() &&
               (melding.back() == '\n' || melding.back() == '\r'))
            melding.pop_back();
        ferdig = spill.checkWin();
    });

    win.add(utforBtn);
    win.add(trekBtn);

    // ── Spilløkke ─────────────────────────────────────────────────────────────
    // Kjører én gang per frame til vinduet lukkes eller spillet er vunnet.
    while (!win.should_close() && !ferdig) {

        // ── Enter-tast (edge-detection) ──
        // Registrerer kun én hendelse per nedtrykk ved å sammenligne med forrige frame
        bool enterNow = win.is_key_down(KeyboardKey::ENTER);
        if (enterNow && !prevEnter)
            prosesser(inputFelt.getText());
        prevEnter = enterNow;

        // ── Museknapp (edge-detection) ──
        // Håndterer klikk for kortflytting. Bygger soner på nytt hvert frame
        // slik at de alltid matcher nåværende bretttilstand.
        bool mouseDown = win.is_left_mouse_button_down();
        if (mouseDown && !flyttTilstand.prevMouseDown) {
            TDT4102::Point musPos = win.get_mouse_coordinates();
            auto soner = Flytt::byggSoner(spill);
            Flytt::haandterKlikk(spill, flyttTilstand, soner, musPos.x, musPos.y);
            melding = flyttTilstand.melding;
            ferdig = spill.checkWin();
        }
        flyttTilstand.prevMouseDown = mouseDown;

        // ── Tegning ──
        GUI::drawBoard(win, spill, kortBilder);

        // Tegn gul ramme rundt valgt kort (vises ingenting dersom ingen er valgt)
        Flytt::tegnMarkering(win, flyttTilstand, spill);

        // Instruksjonstekst over inputfeltet
        win.draw_text({GUI::MARGIN, GUI::INPUT_Y - 20},
            "Kommando  (eks: H1 H2 CA  eller  trekk)  |  Klikk kort for å flytte",
            Color::white, 13);

        // Vis tilbakemeldingsmelding under inputfeltet (gul tekst)
        if (!melding.empty())
            win.draw_text({GUI::MARGIN, GUI::INPUT_Y + 48}, melding, Color{255, 220, 60}, 15);

        win.next_frame(); // Send ferdig frame til skjermen
    }

    // ── Seiersskjerm ──────────────────────────────────────────────────────────
    // Vises i 300 frames (~5 sekunder) når spilleren vinner
    if (ferdig && !win.should_close()) {
        for (int i = 0; i < 300 && !win.should_close(); ++i) {
            GUI::drawBoard(win, spill, kortBilder);
            // Svart boks med gull-ramme i midten
            win.draw_rectangle({GUI::WIN_W/2 - 240, GUI::WIN_H/2 - 60}, 480, 120,
                               Color::black, Color::gold);
            // Gratulasjonstekst i gull
            win.draw_text({GUI::WIN_W/2 - 195, GUI::WIN_H/2 - 18},
                          "Gratulerer - du vant!", Color::gold, 32);
            win.next_frame();
        }
    }

    return 0;
}
