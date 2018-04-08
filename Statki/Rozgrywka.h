#ifndef Rozgrywka_h
#define Rozgrywka_h

#include "Gracz.h"

enum StanGry{SG_NieRozpoczeta,SG_RuchGracza,SG_Wygrana};
enum TrybGry{TG_UserUser,TG_UserComp,TG_CompComp};


class Rozgrywka
{
    Gracz g1,g2;
    Gracz *AktualnyGracz;
    Gracz *GraczAtakowany;
public:
    bool StartGry();
    bool Strzal();
    bool KoniecGry();
    void statystyki();

};

#endif
