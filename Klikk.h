
#pragma once
#include<GUI.h>

//Vil at første klikk skal velge hvilken haug og da hvilket kort, andre klikket skal velge destinasjon

namespace Flytt {
    //definere "soner" det som blir knapper eller steder å trykke
    struct Sone {
        int x,y;
        int w,h;
        std::string heapID;
        int kortIndex;
    };

    //FyttTIlstand

    struct FlyttTilstand {
        bool harValgt = false;
        std::string fraID;
        Card valgKort;
        int valgtKortIndex = -1;
        std::string melding;

        bool prevMouseDown = false;
    };

    std::vector<Sone> lagSoner(Kabal& spill);

    bool treffSone(const Sone& sone, int mx, int my);

    const Sone* finnKlikketSone(const std::vector<Sone>& soner, int mx, int my);

}