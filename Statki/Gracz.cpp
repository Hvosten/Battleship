#include <iostream>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include "Gracz.h"

using namespace std;
extern HANDLE uchwyt;

void gotoxy( int column, int line )///PORUSSZANIE SIE PO KONSOLI
{
      COORD coord;
      coord.X = column;
      coord.Y = line;
      SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
}

COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)///ZWRACA POLOZENIE KURSORA
{
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
    {
        return cbsi.dwCursorPosition;
    }
    else
    {
        // The function failed. Call GetLastError() for details.
        COORD invalid = { 0, 0 };
        return invalid;
    }
}

Gracz::Gracz()
{
    liczba_pol_trafionych = 0;
    liczba_pudel = 0;
    czy_komputer = false;
    ostatni_strzal.x = 0;
    ostatni_strzal.y = 0;
    ilosc_statkow[0] = 4;
    ilosc_statkow[1] = 3;
    ilosc_statkow[2] = 2;
    ilosc_statkow[3] = 1;
}

bool Gracz::pobierz_dane(int ktory_gracz)
{
    cout<<"Podaj imie "<<ktory_gracz<<" gracza:";
    cin>>imie;

    cout<<imie<<" co chcesz zrobic:"<<endl;
    cout<<"1. Pobrac plansze z pliku gracz"<<ktory_gracz<<".txt"<<endl;
    cout<<"2. Wygenerowac losowa plansze"<<endl;
    cout<<"Decyzja:";
    int d;
    cin>>d;
    switch(d)
    {
    case 1:
        {
            fstream plik;
            if(ktory_gracz == 1)
                plik.open("gracz1.txt",ios::in);
            else
                plik.open("gracz2.txt",ios::in);
            if(!plik.is_open())
            {
                cout<<"Nie udalo sie otworzyc pliku "<<ktory_gracz<<" gracza!"<<endl;
                return false;
            }
            string linia;
            for(int i = 0 ; i < 10 ; ++i)
            {
                getline(plik,linia);
                for(int j = 0 ; j < 10 ; ++j)
                {
                    if(linia[j] == '*')
                        plansza[i][j] = POLE_PUSTE;
                    else
                        plansza[i][j] = POLE_STATEK;
                }
            }
            plik.close();
            if(!poprawnosc_planszy())
                return false;
            break;
        }
    case 2:
        wygeneruj_plansze();
        break;
    default:
        cout<<"Nie ma takiej opcji"<<endl;
        return false;
    }
    return true;
}

void Gracz::wyswietl_plansze(int przesuniecie)
{
    //system("cls");
    gotoxy(30,przesuniecie);
    SetConsoleTextAttribute(uchwyt,8);
    cout<<static_cast<char>(219);
    SetConsoleTextAttribute(uchwyt,15);
    for(int i = 0 ; i < 10 ; ++i)
        cout<<i;
    for(int i = 0 ; i < 10 ; ++i,cout<<endl)
    {
        gotoxy(30,przesuniecie+1+i);
        SetConsoleTextAttribute(uchwyt,15);
        cout<<static_cast<char>(65+i);
        for(int j = 0 ; j < 10 ; ++j)
            wyswietl_pole(i,j);
    }
    gotoxy(30,11+przesuniecie);
    SetConsoleTextAttribute(uchwyt,11);
    cout<<"Plansza gracza "<<endl;
    gotoxy(30,12+przesuniecie);
    cout<<get_imie()<<endl;
    wyswietl_legende();
    gotoxy(0,0);
    SetConsoleTextAttribute(uchwyt,15);
    //COORD C = GetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ));


}

void Gracz::wyswietl_legende()
{
    gotoxy(0,5);
    SetConsoleTextAttribute(uchwyt,15);
    cout<<"-----------------------------"<<endl;
    cout<<"GRA STATKI"<<endl;
    cout<<"Wybieraj pola strzalkami"<<endl;
    cout<<"Zatwierdzaj enterem"<<endl;
    cout<<"Spacja odzaczaj pola\ngdzie na pewno nie ma statkow"<<endl;
    cout<<"Legenda:"<<endl;
    SetConsoleTextAttribute(uchwyt,12);
    cout<<static_cast<char>(POLE_ZATOPIONE);
    SetConsoleTextAttribute(uchwyt,15);
    cout<<" - statek zatopiony"<<endl;
    SetConsoleTextAttribute(uchwyt,12);
    cout<<static_cast<char>(POLE_TRAFIONE);
    SetConsoleTextAttribute(uchwyt,15);
    cout<<" - statek trafiony"<<endl;
    SetConsoleTextAttribute(uchwyt,14);
    cout<<static_cast<char>(POLE_PUDLO);
    SetConsoleTextAttribute(uchwyt,15);
    cout<<" - pudlo"<<endl;
    SetConsoleTextAttribute(uchwyt,8);
    cout<<static_cast<char>(POLE_ODZNACZONE);
    SetConsoleTextAttribute(uchwyt,15);
    cout<<" - pole odznaczone"<<endl;
    cout<<"-----------------------------"<<endl;
}

void Gracz::wyswietl_pole(int i,int j)
{
    switch(plansza[i][j])
    {
    case POLE_PUDLO:
        SetConsoleTextAttribute(uchwyt,14);
        cout<<static_cast<char>(plansza[i][j]);
        break;
    case POLE_PUSTE:
        SetConsoleTextAttribute(uchwyt,9);
        cout<<static_cast<char>(plansza[i][j]);
        break;
    case POLE_STATEK:
        SetConsoleTextAttribute(uchwyt,9);
        cout<<static_cast<char>(POLE_PUSTE);
        break;
    case POLE_TRAFIONE:
        SetConsoleTextAttribute(uchwyt,12);
        cout<<static_cast<char>(plansza[i][j]);
        break;
    case POLE_ZATOPIONE:
        SetConsoleTextAttribute(uchwyt,12);
        cout<<static_cast<char>(plansza[i][j]);
        break;
    case POLE_ODZNACZONE:
        SetConsoleTextAttribute(uchwyt,8);
        cout<<static_cast<char>(plansza[i][j]);
        break;
    }
}

void Gracz::wyswietl_plansze_na_koniec_gry(int przesuniecie)
{
    gotoxy(30,przesuniecie);
    SetConsoleTextAttribute(uchwyt,8);
    cout<<static_cast<char>(219);
    SetConsoleTextAttribute(uchwyt,15);
    for(int i = 0 ; i < 10 ; ++i)
        cout<<i;
    for(int i = 0 ; i < 10 ; ++i,cout<<endl)
    {
        gotoxy(30,przesuniecie+1+i);
        SetConsoleTextAttribute(uchwyt,15);
        cout<<static_cast<char>(65+i);
        for(int j = 0 ; j < 10 ; ++j)
        {
            SetConsoleTextAttribute(uchwyt,10);
            if(plansza[i][j] == POLE_STATEK)
                cout<<static_cast<char>(POLE_STATEK);
            else
                wyswietl_pole(i,j);
        }
    }
    gotoxy(30,11+przesuniecie);
    SetConsoleTextAttribute(uchwyt,11);
    cout<<"Plansza gracza "<<endl;
    gotoxy(30,12+przesuniecie);
    cout<<get_imie()<<endl;
    wyswietl_legende();
    gotoxy(0,0);
    SetConsoleTextAttribute(uchwyt,15);

}

bool Gracz::sprawdz_sasiada(int i,int j,int &licznik,char **sprawdzone)
{
    if(j+1 < 10  && sprawdzone[i][j+1] == 'N')///SPRAWDZAMY CZY STATEK CIAGNIE SIE NA PRAWO
    {
        sprawdzone[i][j+1] = 'C';
        if(plansza[i][j+1] == POLE_STATEK)
            if(!sprawdz_sasiada(i,j+1,++licznik,sprawdzone))
                return false;
    }
    if(j-1 >= 0  && sprawdzone[i][j-1] == 'N')///SPRAWDZAMY CZY STATEK CIAGNIE SIE NA LEWO
    {
        sprawdzone[i][j-1] = 'C';
        if(plansza[i][j-1] == POLE_STATEK)
            if(!sprawdz_sasiada(i,j-1,++licznik,sprawdzone))
                return false;
    }
    if(i+1 < 10  && sprawdzone[i+1][j] == 'N')///SPRAWDZAMY CZY STATEK CIAGNIE SIE NA DOL
    {
        sprawdzone[i+1][j] = 'C';
        if(plansza[i+1][j] == POLE_STATEK)
            if(!sprawdz_sasiada(i+1,j,++licznik,sprawdzone))
                return false;
    }

    ///SRAWDZENIE CZY STATKI SIE NIE STYKAJA
    if(i-1 >= 0 && j-1 >= 0 && plansza[i-1][j] == POLE_PUSTE && plansza[i][j-1] == POLE_PUSTE && plansza[i-1][j-1] == POLE_STATEK)
        return false;

    if(i-1 >= 0 && j+1 < 10 && plansza[i-1][j] == POLE_PUSTE && plansza[i][j+1] == POLE_PUSTE && plansza[i-1][j+1] == POLE_STATEK)
        return false;

    if(i+1 < 10 && j+1 < 10 && plansza[i+1][j] == POLE_PUSTE && plansza[i][j+1] == POLE_PUSTE && plansza[i+1][j+1] == POLE_STATEK)
        return false;

    if(i+1 < 10 && j-1 >= 0 && plansza[i+1][j] == POLE_PUSTE && plansza[i][j-1] == POLE_PUSTE && plansza[i+1][j-1] == POLE_STATEK)
        return false;

    return true;
}

bool Gracz::poprawnosc_planszy()
{
    int statki[4] = {0,0,0,0};
    int licznik;
    char **sprawdzone;
    sprawdzone = new char*[10];
    for(int i = 0 ; i < 10 ; ++i)
    {
        sprawdzone[i] = new char[10];
        for(int j = 0 ; j < 10 ; ++j)
            sprawdzone[i][j] = 'N';///N - NO CHECKED
    }


    for(int i = 0 ; i < 10 ; ++i)
        for(int j = 0 ; j < 10 ; ++j)
            if(sprawdzone[i][j] == 'N')
            {
                sprawdzone[i][j] = 'C';///C - CHECKED
                licznik = 0;
                if(plansza[i][j] == POLE_STATEK)
                {
                    if(!sprawdz_sasiada(i,j,++licznik,sprawdzone))
                    {
                        cout<<"Bledna plansza gracza: "<<imie<<endl;
                        cout<<"Statki stykaja sie!"<<endl;
                        return false;
                    }
                    if(licznik > 4)
                    {
                        cout<<"Bledna plansza gracza: "<<imie<<endl;
                        cout<<"Za duzy rozmiar jednego ze statkow!"<<endl;
                        return false;
                    }
                    else
                        ++statki[licznik-1];
                }
            }
    if(statki[0] != 4 || statki[1] != 3 || statki[2] != 2 || statki[3] != 1)
    {
        cout<<"Bledna plansza gracza: "<<imie<<endl;
        cout<<"Dodales za duzo lub za malo statkow!"<<endl;
        return false;
    }


    for(int i = 0 ; i < 10 ; ++i)
        delete [] sprawdzone[i];
    delete [] sprawdzone;
    return true;
}

bool Gracz::czy_sasiad_zatopiony(int i,int j,char** sprawdzone)
{
    sprawdzone[i][j] = 'C';
    if(j-1 >= 0 && plansza[i][j-1] == POLE_STATEK)
        return false;
    else
    {
        if(j-1 >= 0 && plansza[i][j-1] == POLE_TRAFIONE && sprawdzone[i][j-1] == 'N')
            if(!czy_sasiad_zatopiony(i,j-1,sprawdzone))
                return false;
    }

    if(i-1 >= 0 && plansza[i-1][j] == POLE_STATEK)
        return false;
    else
    {
        if(i-1 >= 0 && plansza[i-1][j] == POLE_TRAFIONE && sprawdzone[i-1][j] == 'N')
            if(!czy_sasiad_zatopiony(i-1,j,sprawdzone))
                return false;
    }

    if(i+1 < 10 && plansza[i+1][j] == POLE_STATEK)
        return false;
    else
    {
        if(i+1 < 10 && plansza[i+1][j] == POLE_TRAFIONE && sprawdzone[i+1][j] == 'N')
            if(!czy_sasiad_zatopiony(i+1,j,sprawdzone))
                return false;
    }

    if(j+1 < 10 && plansza[i][j+1] == POLE_STATEK)
        return false;
    else
    {
        if(j+1 < 10 && plansza[i][j+1] == POLE_TRAFIONE && sprawdzone[i][j+1] == 'N')
            if(!czy_sasiad_zatopiony(i,j+1,sprawdzone))
                return false;
    }

    return true;
}

bool Gracz::czy_zatopiony(int m,int n)
{
    char **sprawdzone;
    sprawdzone = new char*[10];
    for(int i = 0 ; i < 10 ; ++i)
    {
        sprawdzone[i] = new char[10];
        for(int j = 0 ; j < 10 ; ++j)
            sprawdzone[i][j] = 'N';
    }

    if(!czy_sasiad_zatopiony(m,n,sprawdzone))
    {
        for(int i = 0 ; i < 10 ; ++i)
            delete [] sprawdzone[i];
        delete [] sprawdzone;
        return false;
    }

    int rozmiar = 0;
    for(int i = 0 ; i < 10 ; ++i)
        for(int j = 0 ; j < 10 ; ++j)
            if(sprawdzone[i][j] == 'C')
            {
                ++rozmiar;
                plansza[i][j] = POLE_ZATOPIONE;
            }

    --ilosc_statkow[rozmiar-1];
    for(int i = 0 ; i < 10 ; ++i)
        delete [] sprawdzone[i];
    delete [] sprawdzone;
    return true;
}

void Gracz::wyczysc_plansze()
{
    for(int i = 0 ; i < 10 ; ++i)
        for(int j = 0 ; j < 10 ; ++j)
            plansza[i][j] = POLE_PUSTE;
}

bool Gracz::umiesc_statek(int& rozmiar,int w,int k)
{
    if(plansza[w][k] == POLE_STATEK)
        return false;

    char **sprawdzone;
    sprawdzone = new char*[10];
    for(int i = 0 ; i < 10 ; ++i)
    {
        sprawdzone[i] = new char[10];
        for(int j = 0 ; j < 10 ; ++j)
            sprawdzone[i][j] = 'N';
    }
    if(czy_mozna_umiescic(rozmiar,w,k,sprawdzone))
    {
        for(int i = 0 ; i < 10 ; ++i)
            for(int j = 0 ; j < 10 ; ++j)
                if(sprawdzone[i][j] == 'C')
                    plansza[i][j] = POLE_STATEK;

        for(int i = 0 ; i < 10 ; ++i)
            delete [] sprawdzone[i];
        delete [] sprawdzone;
        return true;
    }
    else
    {
        for(int i = 0 ; i < 10 ; ++i)
            delete [] sprawdzone[i];
        delete [] sprawdzone;
        return false;
    }

}

bool Gracz::czy_mozna_umiescic(int &rozmiar,int w,int k,char **sprawdzone)
{
    if(!sprawdz_sasiednie_pola(w,k,POLE_STATEK))
        return false;

    sprawdzone[w][k] = 'C';
    --rozmiar;
    if(!rozmiar)
        return true;
    int kierunek[4] = {1,1,1,1};
    if(!w)
        kierunek[0] = 0;
    if(k == 9)
        kierunek[1] = 0;
    if(w == 9)
        kierunek[2] = 0;
    if(!k)
        kierunek[3] = 0;

    do
    {
        int strona = rand()%4;
        if(kierunek[strona])
            switch(strona)
            {
            case 0:///GORA
                if(w-1 >= 0 && sprawdzone[w-1][k] == 'N')
                    if(czy_mozna_umiescic(rozmiar,w-1,k,sprawdzone))
                        return true;
                kierunek[strona] = 0;
                break;
            case 1:///PRAWO
                if(k+1 < 10 && sprawdzone[w][k+1] == 'N')
                     if(czy_mozna_umiescic(rozmiar,w,k+1,sprawdzone))
                        return true;
                kierunek[strona] = 0;
                break;
            case 2:///DOL
                if(w+1 < 10 && sprawdzone[w+1][k] == 'N')
                    if(czy_mozna_umiescic(rozmiar,w+1,k,sprawdzone))
                        return true;
                kierunek[strona] = 0;
                break;
            case 3:///LEWO
                if(k-1 >= 0 && sprawdzone[w][k-1] == 'N')
                     if(czy_mozna_umiescic(rozmiar,w,k-1,sprawdzone))
                        return true;
                kierunek[strona] = 0;
            }
    }
    while(kierunek[0] || kierunek[1] || kierunek[2] || kierunek[3]);

    return false;

}

void Gracz::wygeneruj_plansze()
{
    int wylosowana,rozmiar,pom;
    int w,k;
    do
    {
        wyczysc_plansze();
        int statki[4] = {4,3,2,1};
        while(statki[0] != 0 || statki[1] != 0 || statki[2] != 0 || statki[3] != 0)
        {
            do
            {
                rozmiar = rand()%4;
            }
            while(!statki[rozmiar]);
            do
            {
                wylosowana = rand()%100;
                w = wylosowana/10;
                k = wylosowana%10;
                pom = rozmiar+1;
            }
            while(!umiesc_statek(pom,w,k));
            --statki[rozmiar];
        }

    }
    while(!poprawnosc_planszy());
}

bool Gracz::sprawdz_sasiednie_pola(int w,int k,POLE p)
{
    if(plansza[w][k] == POLE_PUDLO || plansza[w][k] == p)
        return false;

    if(w-1 >= 0 && plansza[w-1][k] == p)
        return false;

    if(k+1 < 10 && plansza[w][k+1] == p)
        return false;

    if(w+1 < 10 && plansza[w+1][k] == p)
        return false;

    if(k-1 >= 0 && plansza[w][k-1] == p)
        return false;

    if(w-1 >= 0 && k-1 >= 0 && plansza[w-1][k-1] == p)
        return false;

    if(w-1 >= 0 && k+1 < 10 && plansza[w-1][k+1] == p)
        return false;

    if(w+1 < 10 && k-1 >= 0 && plansza[w+1][k-1] == p)
        return false;

    if(w+1 < 10 && k+1 < 10 && plansza[w+1][k+1] == p)
        return false;
    return true;
}

strzal Gracz::ruch_komputera()
{
    bool czy_trafiony = false;
    char **sprawdzone;
    sprawdzone = new char*[10];

    for(int i = 0 ; i < 10 ; ++i)
    {
        sprawdzone[i] = new char[10];
        for(int j = 0 ; j < 10 ; ++j)
        {
            if(plansza[i][j] == POLE_TRAFIONE)
            {
                sprawdzone[i][j] = 'T';
                czy_trafiony = true;
            }
            else
                sprawdzone[i][j] = 'N';
        }
    }

    int wylosowana,w,k,mozliwe_pola = 0;
    strzal s;
    if(czy_trafiony)
    {
        for(int i = 0 ; i < 10 ; ++i)
            for(int j = 0 ; j < 10 ; ++j)
            {
               if(sprawdzone[i][j] == 'T')
               {
                   if(i-1 >= 0 && sprawdzone[i-1][j] == 'N')
                   {
                        if(sprawdz_sasiednie_pola(i-1,j,POLE_ZATOPIONE))
                        {
                            ++mozliwe_pola;
                            sprawdzone[i-1][j] = 'M';
                        }
                        else
                            sprawdzone[i-1][j] = 'C';
                   }

                   if(i+1 < 10 && sprawdzone[i+1][j] == 'N')
                   {
                        if(sprawdz_sasiednie_pola(i+1,j,POLE_ZATOPIONE))
                        {
                            ++mozliwe_pola;
                            sprawdzone[i+1][j] = 'M';
                        }
                        else
                            sprawdzone[i+1][j] = 'C';
                   }

                   if(j+1 < 10 && sprawdzone[i][j+1] == 'N')
                   {
                        if(sprawdz_sasiednie_pola(i,j+1,POLE_ZATOPIONE))
                        {
                            ++mozliwe_pola;
                            sprawdzone[i][j+1] = 'M';
                        }
                        else
                            sprawdzone[i][j+1] = 'C';
                   }

                   if(j-1 >= 0 && sprawdzone[i][j-1] == 'N')
                   {
                        if(sprawdz_sasiednie_pola(i,j-1,POLE_ZATOPIONE))
                        {
                            ++mozliwe_pola;
                            sprawdzone[i][j-1] = 'M';
                        }
                        else
                            sprawdzone[i][j-1] = 'C';
                   }

               }

            }
            wylosowana = rand()%mozliwe_pola + 1;
            for(int i = 0 ; i < 10 ; ++i)
                for(int j = 0 ; j < 10 ; ++j)
                {
                    if(sprawdzone[i][j] == 'M')
                        --wylosowana;
                    if(!wylosowana)
                    {
                        s.x = i;
                        s.y = j;
                        return s;
                    }
                }
    }
    else
    {
        do
        {
            wylosowana = rand()%100;
            w = wylosowana/10;
            k = wylosowana%10;
        }
        while(!sprawdz_sasiednie_pola(w,k,POLE_ZATOPIONE));///CZY NIE MA KOLIZJI
        s.x = w;
        s.y = k;
        return s;
    }
}

strzal Gracz::wybierz_pole()
{
    gotoxy(ostatni_strzal.y+31,ostatni_strzal.x+1);
    SetConsoleTextAttribute(uchwyt,13);
    if(plansza[ostatni_strzal.x][ostatni_strzal.y] != POLE_STATEK)
        cout<<static_cast<char>(plansza[ostatni_strzal.x][ostatni_strzal.y]);
    else
        cout<<static_cast<char>(POLE_PUSTE);

    gotoxy(0,1);
    SetConsoleTextAttribute(uchwyt,15);
    cout<<"Strzel w pole ";
    do
    {
        gotoxy(14,1);
        SetConsoleTextAttribute(uchwyt,15);
        cout<<(char)('A'+ostatni_strzal.x)<<ostatni_strzal.y;
        if((GetAsyncKeyState(VK_RIGHT)& 0x8000)&&(ostatni_strzal.y<9))
        {
            gotoxy(ostatni_strzal.y+31,ostatni_strzal.x+1);
            wyswietl_pole(ostatni_strzal.x,ostatni_strzal.y);
            gotoxy(++ostatni_strzal.y+31,ostatni_strzal.x+1);
            SetConsoleTextAttribute(uchwyt,13);
            if(plansza[ostatni_strzal.x][ostatni_strzal.y] != POLE_STATEK)
                cout<<static_cast<char>(plansza[ostatni_strzal.x][ostatni_strzal.y]);
            else
                cout<<static_cast<char>(POLE_PUSTE);
            Sleep(150);
        }
        if((GetAsyncKeyState(VK_LEFT)& 0x8000)&&(0<ostatni_strzal.y))
        {
            gotoxy(ostatni_strzal.y+31,ostatni_strzal.x+1);
            wyswietl_pole(ostatni_strzal.x,ostatni_strzal.y);
            gotoxy(--ostatni_strzal.y+31,ostatni_strzal.x+1);
            SetConsoleTextAttribute(uchwyt,13);
            if(plansza[ostatni_strzal.x][ostatni_strzal.y] != POLE_STATEK)
                cout<<static_cast<char>(plansza[ostatni_strzal.x][ostatni_strzal.y]);
            else
                cout<<static_cast<char>(POLE_PUSTE);
            Sleep(150);
        }
        if((GetAsyncKeyState(VK_UP)& 0x8000)&&(0<ostatni_strzal.x))
        {
            gotoxy(ostatni_strzal.y+31,ostatni_strzal.x+1);
            wyswietl_pole(ostatni_strzal.x,ostatni_strzal.y);
            gotoxy(ostatni_strzal.y+31,--ostatni_strzal.x+1);
            SetConsoleTextAttribute(uchwyt,13);
            if(plansza[ostatni_strzal.x][ostatni_strzal.y] != POLE_STATEK)
                cout<<static_cast<char>(plansza[ostatni_strzal.x][ostatni_strzal.y]);
            else
                cout<<static_cast<char>(POLE_PUSTE);
            Sleep(150);
        }
        if((GetAsyncKeyState(VK_DOWN)& 0x8000)&&(ostatni_strzal.x<9))
        {
            gotoxy(ostatni_strzal.y+31,ostatni_strzal.x+1);
            wyswietl_pole(ostatni_strzal.x,ostatni_strzal.y);
            gotoxy(ostatni_strzal.y+31,++ostatni_strzal.x+1);
            SetConsoleTextAttribute(uchwyt,13);
            if(plansza[ostatni_strzal.x][ostatni_strzal.y] != POLE_STATEK)
                cout<<static_cast<char>(plansza[ostatni_strzal.x][ostatni_strzal.y]);
            else
                cout<<static_cast<char>(POLE_PUSTE);
            Sleep(150);
        }
        if((GetAsyncKeyState(VK_SPACE)& 0x8000)&&(czy_sasiednie_pole_zatopione(ostatni_strzal.x,ostatni_strzal.y))&&
           plansza[ostatni_strzal.x][ostatni_strzal.y] != POLE_ODZNACZONE)
        {
            plansza[ostatni_strzal.x][ostatni_strzal.y] = POLE_ODZNACZONE;
            gotoxy(ostatni_strzal.y+31,ostatni_strzal.x+1);
            wyswietl_pole(ostatni_strzal.x,ostatni_strzal.y);
            Sleep(150);
        }
    }
    while(!(GetAsyncKeyState(VK_RETURN)& 0x8000));
    return ostatni_strzal;
}

bool Gracz::czy_sasiednie_pole_zatopione(int w,int k)
{
    if(plansza[w][k] == POLE_PUDLO || plansza[w][k] == POLE_STATEK || plansza[w][k] == POLE_TRAFIONE ||
       plansza[w][k] == POLE_ZATOPIONE)
        return false;

    if(w-1 >= 0 && plansza[w-1][k] == POLE_ZATOPIONE)
        return true;

    if(k+1 < 10 && plansza[w][k+1] == POLE_ZATOPIONE)
        return true;

    if(w+1 < 10 && plansza[w+1][k] == POLE_ZATOPIONE)
        return true;

    if(k-1 >= 0 && plansza[w][k-1] == POLE_ZATOPIONE)
        return true;

    if(w-1 >= 0 && k-1 >= 0 && plansza[w-1][k-1] == POLE_ZATOPIONE)
        return true;

    if(w-1 >= 0 && k+1 < 10 && plansza[w-1][k+1] == POLE_ZATOPIONE)
        return true;

    if(w+1 < 10 && k-1 >= 0 && plansza[w+1][k-1] == POLE_ZATOPIONE)
        return true;

    if(w+1 < 10 && k+1 < 10 && plansza[w+1][k+1] == POLE_ZATOPIONE)
        return true;
    return false;
}
