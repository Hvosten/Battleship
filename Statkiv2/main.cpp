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

int main()
{
    srand ((time(NULL)));
    SetConsoleTextAttribute(uchwyt,15);
    cout<<"GRA STATKI"<<endl;
    cout<<"Wybierz rodzaj rozgrywki:"<<endl;
    cout<<"1.User vs User"<<endl;
    cout<<"2.User vs Komputer"<<endl;
    cout<<"3.Gra pokazowa(Komputer vs Komputer)"<<endl;
    cout<<"Decyzja:";
    int d;
    cin>>d;
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
