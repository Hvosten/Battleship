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
extern void gotoxy(int,int);

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
            system("cls");
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

    if(rand() % 2 == 0)
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
    Sleep(500);
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
        system("cls");
        statystyki();
        GraczAtakowany->wyswietl_plansze();
        AktualnyGracz->wyswietl_plansze(14);
        SetConsoleTextAttribute(uchwyt,11);
        cout<<AktualnyGracz->get_imie();
        SetConsoleTextAttribute(uchwyt,15);
        cout<<" strzela w pole ";
        cout<<(char)('A'+s.x);
        cout<<s.y<<endl;
        Sleep(1000);
    }
    else
    {
        system("cls");
        statystyki();
        GraczAtakowany->wyswietl_plansze();
        AktualnyGracz->wyswietl_plansze(14);
        SetConsoleTextAttribute(uchwyt,11);
        cout<<AktualnyGracz->get_imie();
        SetConsoleTextAttribute(uchwyt,15);
        cout<<" - wybierz pole:"<<endl;
        s = GraczAtakowany->wybierz_pole();
        while(GraczAtakowany->get_el_planszy(s.x,s.y) == POLE_PUDLO || GraczAtakowany->get_el_planszy(s.x,s.y) == POLE_TRAFIONE ||
              GraczAtakowany->get_el_planszy(s.x,s.y) == POLE_ZATOPIONE)
        {
            GraczAtakowany->wyswietl_plansze();
            cout<<"Bledne pole, podaj poprawne:";
            s = GraczAtakowany->wybierz_pole();
        }
    }

    if(GraczAtakowany->get_el_planszy(s.x,s.y) == POLE_STATEK)
    {
        GraczAtakowany->zmien_el_planszy(POLE_TRAFIONE,s.x,s.y);
        bool flaga = GraczAtakowany->czy_zatopiony(s.x,s.y);
        system("cls");
        statystyki();
        GraczAtakowany->wyswietl_plansze();
        AktualnyGracz->wyswietl_plansze(14);
        if(flaga)
            cout<<"TRAFIONY I ZATOPIONY!"<<endl;
        else
            cout<<"TRAFIONY!"<<endl;
        AktualnyGracz->zwieksz_liczbe_pol_trafionych();
        if(AktualnyGracz->get_liczba_pol_trafionych() == 20)
            stan = SG_Wygrana;
        else
        {
            SetConsoleTextAttribute(uchwyt,11);
            cout<<AktualnyGracz->get_imie();
            SetConsoleTextAttribute(uchwyt,15);
            cout<<" nadal Twoj ruch"<<endl;
        }
        Sleep(1000);
    }

    if(GraczAtakowany->get_el_planszy(s.x,s.y) == POLE_PUSTE ||
       GraczAtakowany->get_el_planszy(s.x,s.y) == POLE_ODZNACZONE)
    {
        GraczAtakowany->zmien_el_planszy(POLE_PUDLO,s.x,s.y);
        AktualnyGracz->zwieksz_liczbe_pudel();
        system("cls");
        statystyki();
        GraczAtakowany->wyswietl_plansze();
        AktualnyGracz->wyswietl_plansze(14);
        cout<<"PUDLO!"<<endl;
        SetConsoleTextAttribute(uchwyt,11);
        cout<<AktualnyGracz->get_imie();
        SetConsoleTextAttribute(uchwyt,15);
        cout<<" tracisz kolejke."<<endl;
        Gracz *pom = GraczAtakowany;
        GraczAtakowany = AktualnyGracz;
        AktualnyGracz = pom;
        Sleep(1000);
    }
    return true;
}

bool Rozgrywka::KoniecGry()
{
    if(stan != SG_Wygrana)
        return false;

    system("cls");
    gotoxy(0,0);
    SetConsoleTextAttribute(uchwyt,10);
    cout<<"WYGRYWA "<<AktualnyGracz->get_imie()<<endl;
    cout<<"GRATULACJE!!!"<<endl;
    cout<<static_cast<char>(219);
    SetConsoleTextAttribute(uchwyt,15);
    cout<<" - pozostale statki"<<endl;
    AktualnyGracz->wyswietl_plansze_na_koniec_gry();
    GraczAtakowany->wyswietl_plansze_na_koniec_gry(14);
    statystyki();
    SetConsoleTextAttribute(uchwyt,15);
    cout<<endl;
    gotoxy(0,30);
    system("Pause");
    return true;
}

void Rozgrywka::statystyki()
{
    SetConsoleTextAttribute(uchwyt,15);
    int i = 47,j = 4;
    gotoxy(i,j++);
    cout<<"--------------------------------------";
    gotoxy(i,j++);
    cout<<"Statystyki"<<endl;
    gotoxy(i,j);
    cout<<"Gracz";
    gotoxy(i+14,j);
    cout<<g1.get_imie();
    gotoxy(i+26,j++);
    cout<<g2.get_imie();

    gotoxy(i,j);
    cout<<"Trafione(20)";
    gotoxy(i+14,j);
    cout<<g1.get_liczba_pol_trafionych();
    gotoxy(i+26,j++);
    cout<<g2.get_liczba_pol_trafionych();

    gotoxy(i,j);
    cout<<"Pudla(80)";
    gotoxy(i+14,j);
    cout<<g1.get_liczba_pudel();
    gotoxy(i+26,j++);
    cout<<g2.get_liczba_pudel();
    gotoxy(i,j++);
    cout<<"--------------------------------------";

    j++;
    gotoxy(i,j++);
    cout<<"--------------------------------------";
    gotoxy(i,j++);
    cout<<"Pozostale statki";

    gotoxy(i,j);
    cout<<"Gracz";
    gotoxy(i+20,j);
    cout<<g1.get_imie();
    gotoxy(i+32,j++);
    cout<<g2.get_imie();
    gotoxy(i,j);

    cout<<"Czteromasztowiec(1)";
    gotoxy(i+20,j);
    cout<<g1.get_liczba_statkow(3);
    gotoxy(i+32,j++);
    cout<<g2.get_liczba_statkow(3);

    gotoxy(i,j);
    cout<<"Trojmasztowiec(2)";
    gotoxy(i+20,j);
    cout<<g1.get_liczba_statkow(2);
    gotoxy(i+32,j++);
    cout<<g2.get_liczba_statkow(2);

    gotoxy(i,j);
    cout<<"Dwumasztowiec(3)";
    gotoxy(i+20,j);
    cout<<g1.get_liczba_statkow(1);
    gotoxy(i+32,j++);
    cout<<g2.get_liczba_statkow(1);

    gotoxy(i,j);
    cout<<"Jednomasztowiec(4)";
    gotoxy(i+20,j);
    cout<<g1.get_liczba_statkow(0);
    gotoxy(i+32,j++);
    cout<<g2.get_liczba_statkow(0);
    gotoxy(i,j);
    cout<<"--------------------------------------";
}
