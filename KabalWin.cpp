#include <KabalWin.h>


void KabalWin::draw(){
    // Paint board background first, otherwise default window background stays visible.
    draw_rectangle({0, 0}, GUI::WIN_W, GUI::WIN_H, GUI::FELT);

    GUI::drawStock(*this, this->spill.getStock(),this->images);
    GUI::drawWaste(*this, this->spill.getWaste(),this->images);
    GUI::drawFoundations(*this, this->spill.getFoundations(),this->images);

    for (int i = 0; i < 7; ++i) {
        GUI::drawColumn(*this, this->spill.getHeaps().heaps[i],static_cast<int>(i), this->images);
    }

    tegnMarkering();
}

// --- KONSTRUKTØREN ---
KabalWin::KabalWin() 
    : TDT4102::AnimationWindow(50, 50, GUI::WIN_W, GUI::WIN_H, "Kabal"), // 1. Opprett vinduet
      spill(deck)                                                        // 3. Gi stokken til spillet
{
    // Koden inni her kjører rett ETTER at vinduet og objektene er opprettet i minnet.

    // 1. Sett bakgrunnsfargen (bruk din nye gråfarge her!)
    // Hvis AnimationWindow ikke har setBackgroundColor, kan du droppe denne
    // og bare beholde draw_rectangle() i draw-funksjonen din.
    // setBackgroundColor(GUI::FELT); 

    // 2. Finn ut hvilken mappe bildene ligger i (hentet fra din forrige main)
    std::string kortMappe = "playingCardsImages/";
    if (!std::filesystem::exists("playingCardsImages/back_light.png")) {
        std::cerr << "Advarsel: Fant ikke kortbilder på disken!" << std::endl;
    }

    // 3. Last inn alle de 53 bildene
    images.load(kortMappe);
}
void KabalWin::handterKlikk() {
    bool mouseDown = is_left_mouse_button_down();

    if (mouseDown && !prevMouseDown) {
        auto pos = get_mouse_coordinates();
        std::vector<Flytt::Sone> soner = Flytt::lagSoner(spill);

        const Flytt::Sone* klikket = Flytt::finnKlikketSone(soner, pos.x,pos.y);

        if (klikket == nullptr) {
            harValgtKort = false;
            valgtKortIndex = -1;
            melding = "";
        } else {
        std::string id = klikket->heapID;

        if (id == "S") {
            spill.drawFromStock();
            harValgtKort = false;
            valgtKortIndex = -1;
            melding = "Trakk et kort.";
        }
        else {
            if (!harValgtKort) {
                if (klikket->kortIndex != -1) {
                    heap& h = spill.heapFraID(id);
                    
                    if (h.revealed[klikket->kortIndex]) {
                        Card valgtKort = h.cards[klikket->kortIndex];

                        fraHaugID = id;
                        valgtKortIndex = klikket->kortIndex;
                        valgtKortStreng = GUI::cardToKey(valgtKort);
                        harValgtKort = true;

                        melding = "Valgte kort fra " + id;
                    }
                }
            } else {
            if (id == fraHaugID) {
                harValgtKort = false;
                valgtKortIndex = -1;
                melding = "Valg avbrutt.";
            } else {
                spill.executeMove(fraHaugID,id,valgtKortStreng);

                harValgtKort = false;
                valgtKortIndex = -1;

                if (spill.checkWin()) {
                    ferdig = true;
                }
            }
        }
    } 
}
}
prevMouseDown = mouseDown;
}

void KabalWin::tegnMarkering() {
    if (!harValgtKort) return; // Ingenting valgt, ingenting å tegne
    if (fraHaugID.empty()) return;

    // Finn piksel-posisjon til det valgte kortet basert på heap-type
    int x = 0, y = 0;
    int markerH = GUI::CARD_H;

    if (fraHaugID == "W") {
        // Waste: alltid i kolonne 1, øverste rad
        x = GUI::colX(1);
        y = GUI::TOP_Y;
    } else if (fraHaugID[0] == 'F') {
        // Foundation: kolonnene 3–6, øverste rad
        int fi = std::stoi(fraHaugID.substr(1)) - 1;
        x = GUI::colX(3 + fi);
        y = GUI::TOP_Y;
    } else if (fraHaugID[0] == 'H') {
        // Kolonne: y forskyves med OVERLAP per kort
        int col = std::stoi(fraHaugID.substr(1)) - 1;
        x = GUI::colX(col);
        y = GUI::TAB_Y + valgtKortIndex * GUI::OVERLAP;
    }

    // Beregn høyde på markeringsrammen.
    // For kolonner: rammen dekker det valgte kortet og alle kort under (hele stakken).
    // For andre bunker: kun ett kort → full CARD_H.
    if (fraHaugID[0] == 'H') {
        int col = std::stoi(fraHaugID.substr(1)) - 1;
        heap& hp = spill.getHeaps().heaps[col];
        int antallKort = static_cast<int>(hp.cards.size()) - valgtKortIndex;
        markerH = (antallKort - 1) * GUI::OVERLAP + GUI::CARD_H;
    }

    // Tegn en 3 piksler bred gul ramme rundt det valgte kortet
    // ved å tegne tre rektangler som er 1 piksel større for hvert lag
    Color markeringsFarge{255, 220, 0};
    for (int t = 0; t < 3; ++t) {
        draw_rectangle({x - t, y - t}, GUI::CARD_W + 2*t, markerH + 2*t,
                           Color::transparent, markeringsFarge);
    }
}
