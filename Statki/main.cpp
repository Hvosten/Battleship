#include <iostream>
#include <conio.h>
#include "Gracz.h"
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include "Rozgrywka.h"

using namespace std;
HANDLE uchwyt = GetStdHandle(STD_OUTPUT_HANDLE);
extern TrybGry tryb;
extern void gotoxy(int,int);

void wyswietl_menu()
{
    gotoxy(28,5);
    cout<<"GRA STATKI"<<endl;
    gotoxy(16,6);
    cout<<"Wybierz rodzaj rozgrywki:"<<endl;
    gotoxy(16,7);
    cout<<"1.User vs User"<<endl;
    gotoxy(16,8);
    cout<<"2.User vs Komputer"<<endl;
    gotoxy(16,9);
    cout<<"3.Gra pokazowa(Komputer vs Komputer)"<<endl;
    gotoxy(16,10);
    cout<<"Decyzja:";
}

int main()
{
    srand ((time(NULL)));
    SetConsoleTextAttribute(uchwyt,15);
    wyswietl_menu();
    int d;
    cin>>d;
    system("cls");
    gotoxy(0,0);
    Rozgrywka R;
    switch(d)
    {
    case 1:
        {
            tryb = TG_UserUser;
            R.StartGry();
            while(R.Strzal());
            break;
        }
    case 2:
        {
            tryb = TG_UserComp;
            R.StartGry();
            while(R.Strzal());
            break;
        }
    case 3:
        {
            tryb = TG_CompComp;
            R.StartGry();
            while(R.Strzal());
            break;
        }


    default:
        cout<<"Nie ma takiej opcji!"<<endl;
        return 1;
    }
    R.KoniecGry();


    return 0;
}
