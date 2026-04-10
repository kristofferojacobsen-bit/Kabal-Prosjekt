//


#include "std_lib_facilities.h"
#include <KabalWin.h>

//------------------------------------------------------------------------------'


int main() {
    CardDeck deck;
    KabalWin vindu;

    while (!vindu.should_close()) {
        vindu.handterKlikk();
        vindu.draw();

        if (vindu.isFinished()) {
            // Svart boks med gull-ramme i midten
            vindu.draw_rectangle({GUI::WIN_W / 2 - 240, GUI::WIN_H / 2 - 60}, 480, 120,
                                 Color::black, Color::gold);
            // Gratulasjonstekst i gull
            vindu.draw_text({GUI::WIN_W / 2 - 195, GUI::WIN_H / 2 - 18},
                            "Gratulerer - du vant!", Color::gold, 32);
        }

        vindu.next_frame();
    }
 
    return 0;
}

//------------------------------------------------------------------------------
