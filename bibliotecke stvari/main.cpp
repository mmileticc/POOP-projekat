#include <iostream>
#include <limits>

#include "Database.h"

int regularChoice(int low, int high, const string& menu){
    int choice;
    while (true) {
        cout << menu;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Pogresan unos. Molimo unesite broj od " << low << " do " << high << "." << endl;
        }
        else if (choice < low || choice > high) {
            std::cout << "Unos van opsega. Molimo unesite broj od " << low << " do " << high << "." << endl;
        }
        else {
            break;
        }
    }
    return choice;
}


int main() {
    using namespace std;

    string menu = "Unesite redni broj opcije koju zelite da izaberete:\n"
                  "1. Kreiranje nove baze podataka.\n"
                  "2. Otvaranje postojece baze podataka.\n"
                  "0. Kraj programa.\n";

    string menuExit = "Da li ste sigurni da zelite da napustite program?\n"
                      "Pritisnite 1 za DA ili 0 za NE.\n";

    string menuStop = "Da li ste sigurni da zelite da napustite trenutnu bazu podataka?\n"
                      "Pritisnite 1 za DA ili 0 za NE.\n";

    string menuProces = "Unesite:\n"
                        "0 za zatvaranje tekuce baze podataka\n"
                        "1 za sacuvavanje baze podataka\n"
                        "ili text query-ja:\n";

    Database* db = nullptr;
    string input;
    string fajl;
    while (true){
        int choice = regularChoice(0, 2, menu);

        if(choice == 1){  //kreiranje nove baze
            db = new Database();
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Uspesno kreiranje nove baze podataka.\n";
        }
        if(choice == 2) { //ucitavanje postojece baze podataka
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            cout << "Unesite ime (putanju do) fajla u kom se nalazi baza:\n";

            getline(std::cin, fajl);
            ifstream inFile(fajl);
            if(!inFile.is_open()){
                cout << "Neuspesno otvaranje fajla.\n";
                continue;
            }else{
                db = Database::importDB(inFile);
                cout << "Uspesno otvaranje baze podataka.\n";
            }
        }
        if(choice == 0){  //napustanje programa
            int c = regularChoice(0, 1, menuExit);
            if(c){
                cout << "Kraj programa.";
                break;
            }
            else{
                continue;
            }
        }

        //// baza otvorena/kreirana ide dalja obrada konacno  ////
        bool saved = false;

        while(true){
            try {
                cout << menuProces;

                getline(std::cin, input);
                if(input == "0") {
                    int e = regularChoice(0,1,menuStop);
                    if(e){
                        if(!saved){
                            cout << "Da li zelite da sacuvate bazu pre kraja obrade?\n";
                            int c = regularChoice(0,1, "Unesite 0 za NE ili 1 za DA.\n");
                            if(c) {
                                fajl = "fajlic.mile";
                                db->saveDB(fajl);
                            }
                        }
                        break;
                    }
                    else continue;
                }
                if(input == "1"){
                    fajl = "fajlic.mile";
                    saved = db->saveDB(fajl) == "Vasa baza je uspesno sacuvana i moze se kasnije ponovo otvoriti.\n";
                }else{
                    db->query(input);
                    saved = false;
                }
            }
            catch (exception& e){
                cout << e.what();
            }
        }
    }
}