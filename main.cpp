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

        vindu.next_frame();
    }
    return 0;
}

//------------------------------------------------------------------------------
