#include <Klikk.h>

namespace Flytt {
std::vector<Sone> lagSoner(Kabal& spill) {
    std::vector<Sone> soner;

        //trekkebunke - alltid klikkbar, klikk trekker kort
    {
        int x = GUI::colX(0);
        soner.push_back({x,GUI::TOP_Y, GUI::CARD_W,GUI::CARD_H,"S",-1});
    }

    //Waste - bare øverste kortet kan klikkes
    {
        int x = GUI::colX(1);
        heap& w = spill.getWaste();
        if (!w.cards.empty()){
            soner.push_back({x,GUI::TOP_Y,GUI::CARD_W,GUI::CARD_H,"W",static_cast<int>(w.cards.size() -  1)});
        }
    }

    //Foundations alltid mulig p trykke på
    //kortIndex = -1 hvis tom, ellers indeks til øverste kort
    for (int i = 0; i < 4; ++i) {
        int x = GUI::colX(3+i);
        std::string id = "F" + std::to_string(i + 1);
        heap& f = spill.getFoundations()[i];
        soner.push_back({x, GUI::TOP_Y, GUI::CARD_W, GUI::CARD_H,id, f.cards.empty() ? -1 : static_cast<int>(f.cards.size() - 1)});                        
    }

    //for haugene 1-7, hvert kort er egen sone
    for (int i = 0; i < 7; ++i){
        int x = GUI::colX(i);
        std::string id = "H" + std::to_string(i + 1);
        heap& h = spill.getHeaps().heaps[i];
        
        if (h.cards.empty()) {
            //tomt, hele boksen er sone
            soner.push_back({x,GUI::TAB_Y,GUI::CARD_W,GUI::CARD_H,id,-1});
        } else {
            for (int j = 0; j < static_cast<int>(h.cards.size()); j ++) {
                int y = GUI::TAB_Y +(j*GUI::OVERLAP);

                //alle kortene kan klikkes i kun overlap untatt siste som har hele høyden
                int cardH = (j == static_cast<int>(h.cards.size() - 1)) ? GUI::CARD_H : GUI::OVERLAP;
                //vil ikke gjøre skjulte kort klikkbare så:
                if (h.revealed[j]) {
                    soner.push_back({x,y,GUI::CARD_W,cardH,id,j});
                }
            }
        }
    }
    return soner;
}
bool treffSone(const Sone& sone, int mx, int my) {
    return mx >= sone.x && mx < sone.x + sone.w && my >= sone.y && my < sone.y + sone.h;
}

const Sone* finnKlikketSone(const std::vector<Sone>& soner, int mx, int my) {
    for (int i = static_cast<int>(soner.size()) - 1; i >= 0; --i) {
        if (treffSone(soner[i],mx,my)) {
            return &soner[i];
        }

    }
    return nullptr;
}
}