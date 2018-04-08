#include "Rozgrywka.h"
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <windows.h>
#include <conio.h>
#include <time.h>


StanGry stan = SG_NieRozpoczeta;
TrybGry tryb;
extern HANDLE uchwyt;

void czekaj( int sekundy )
{
    clock_t koniec_czekania;
    koniec_czekania = clock() + sekundy * CLOCKS_PER_SEC;
    while( clock() < koniec_czekania ) { }
}

bool Rozgrywka::StartGry()
{
    if(stan != SG_NieRozpoczeta)
        return false;
    ///POBIERANIE DANYCH

    switch(tryb)
    {
    case TG_UserUser:
        {
            if(!g1.pobierz_dane(1))
                return false;

            if(!g2.pobierz_dane(2))
                return false;
            break;
        }
    case TG_UserComp:
        {
            if(!g1.pobierz_dane(1))
                return false;
             g2.set_imie("KOMPUTER");
             g2.wygeneruj_plansze();
             g2.gracz_komputer();
             break;
        }
    case TG_CompComp:
        {
            g1.set_imie("KOMPUTER_1");
            g1.wygeneruj_plansze();
            g1.gracz_komputer();
            g2.set_imie("KOMPUTER_2");
            g2.wygeneruj_plansze();
            g2.gracz_komputer();
            break;
        }
    }



    if (stan != SG_NieRozpoczeta)
        return false;

    if(rand() % 2)
    {
        AktualnyGracz = &g2;
        GraczAtakowany = &g1;
    }
    else
    {
        AktualnyGracz = &g1;
        GraczAtakowany = &g2;
    }
    stan = SG_RuchGracza;
    return true;
}

bool Rozgrywka::Strzal()
{
    if(stan == SG_NieRozpoczeta || stan == SG_Wygrana)
        return false;

    strzal s;
    if(AktualnyGracz->get_czy_komputer())
    {
        s = GraczAtakowany->ruch_komputera();
        GraczAtakowany->wyswietl_plansze();
        cout<<AktualnyGracz->get_imie()<<" strzela w pole";
        SetConsoleTextAttribute(uchwyt,11);
        cout<<(char)('A'+s.x);
        SetConsoleTextAttribute(uchwyt,10);
        cout<<s.y<<endl;
        //getch();
        czekaj(1);
    }
    else
    {
        string decyzja;
        GraczAtakowany->wyswietl_plansze();
        cout<<AktualnyGracz->get_imie()<<" - podaj pole w ktore chcesz strzelic:";
        cin>>decyzja;
        transform(decyzja.begin(),decyzja.end(),decyzja.begin(),::toupper);
        s.x = decyzja[0] - 'A';
        s.y = decyzja[1] - '0';
        while(decyzja[0] < 'A' || decyzja[0] > 'K' || decyzja[1] < '0' || decyzja[1] > '9' ||
              GraczAtakowany->get_el_planszy(s.x,s.y) == POLE_PUDLO || GraczAtakowany->get_el_planszy(s.x,s.y) == POLE_TRAFIONE ||
              GraczAtakowany->get_el_planszy(s.x,s.y) == POLE_ZATOPIONE)
        {
            GraczAtakowany->wyswietl_plansze();
            cout<<"Bledne pole, podaj poprawne:";
            cin>>decyzja;
            transform(decyzja.begin(),decyzja.end(),decyzja.begin(),::toupper);
            s.x = decyzja[0] - 'A';
            s.y = decyzja[1] - '0';
        }
    }

    if(GraczAtakowany->get_el_planszy(s.x,s.y) == POLE_STATEK)
    {
        GraczAtakowany->zmien_el_planszy(POLE_TRAFIONE,s.x,s.y);
        bool flaga = GraczAtakowany->czy_zatopiony(s.x,s.y);
        GraczAtakowany->wyswietl_plansze();
        if(flaga)
            cout<<"TRAFIONY I ZATOPIONY!"<<endl;
        else
            cout<<"TRAFIONY!"<<endl;
        AktualnyGracz->zwieksz_liczbe_pol_trafionych();
        if(AktualnyGracz->get_liczba_pol_trafionych() == 20)
            stan = SG_Wygrana;
        else
            cout<<AktualnyGracz->get_imie()<<" nastepny ruch znow nalezy do Ciebie."<<endl;
        //getch();
        czekaj(1);
    }

    if(GraczAtakowany->get_el_planszy(s.x,s.y) == POLE_PUSTE)
    {
        GraczAtakowany->zmien_el_planszy(POLE_PUDLO,s.x,s.y);
        GraczAtakowany->wyswietl_plansze();
        cout<<"PUDLO!"<<endl;
        cout<<AktualnyGracz->get_imie()<<" tracisz kolejke."<<endl;
        Gracz *pom = GraczAtakowany;
        GraczAtakowany = AktualnyGracz;
        AktualnyGracz = pom;
        //getch();
        czekaj(1);
    }
    return true;
}

bool Rozgrywka::KoniecGry()
{
    if(stan != SG_Wygrana)
        return false;

    system("cls");
    AktualnyGracz->wyswietl_plansze_na_koniec_gry(0);
    GraczAtakowany->wyswietl_plansze_na_koniec_gry(1);
    getch();
    return true;
}
