#ifndef Gracz_h
#define Gracz_h

#include <iostream>

using namespace std;


enum POLE{POLE_PUSTE = 'O',POLE_TRAFIONE = 'H',POLE_ZATOPIONE = 'X',POLE_STATEK = static_cast<char>(219),
POLE_ODZNACZONE = '#',POLE_PUDLO = '*'};
struct strzal
{
    int x;
    int y;
};

class Gracz
{
    string imie;
    POLE plansza[10][10];
    int liczba_pol_trafionych;
    int liczba_pudel;
    bool czy_komputer;
    strzal ostatni_strzal;
    int ilosc_statkow[4];

    bool sprawdz_sasiada(int,int,int&,char**);///METODA DO SPRAWDZANIA POPRAWNOSCI PLANSZY
    bool czy_sasiad_zatopiony(int,int,char**);
    void wyczysc_plansze();
    bool czy_mozna_umiescic(int&,int,int,char**);
    bool umiesc_statek(int&,int,int);
    bool sprawdz_sasiednie_pola(int,int,POLE);
public:
    Gracz();
    bool pobierz_dane(int);
    void wyswietl_plansze(int przesuniecie=0);
    void wyswietl_pole(int,int);
    void wyswietl_legende();
    void wyswietl_plansze_na_koniec_gry(int przesuniecie=0);
    bool poprawnosc_planszy();
    void set_imie(string i){imie = i;}
    void gracz_komputer(){czy_komputer=true;}
    bool get_czy_komputer(){return czy_komputer;}
    string get_imie(){return imie;}
    POLE get_el_planszy(int i,int j)const{return plansza[i][j];}
    void zmien_el_planszy(POLE p,int i,int j){plansza[i][j] = p;}
    int get_liczba_pol_trafionych(){return liczba_pol_trafionych;}
    int get_liczba_pudel(){return liczba_pudel;}
    int get_liczba_statkow(int i){return ilosc_statkow[i];}
    void zwieksz_liczbe_pol_trafionych(){++liczba_pol_trafionych;}
    void zwieksz_liczbe_pudel(){++liczba_pudel;}
    bool czy_zatopiony(int,int);
    void wygeneruj_plansze();
    strzal ruch_komputera();
    strzal wybierz_pole();
    bool czy_sasiednie_pole_zatopione(int,int);
};

#endif // Gracz_h

