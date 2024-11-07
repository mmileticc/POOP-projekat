//
// Created by Miletic on 4/23/2024.
//

#ifndef POOP_PROJEKAT_1_MYEXEPTIONS_H
#define POOP_PROJEKAT_1_MYEXEPTIONS_H
#include "exception"
#include <memory>
#include <utility>

using namespace std;
class ECantDelete :public exception{
private:
    string message;
public:
    explicit ECantDelete(int i): exception(){
        message = "Greska! Nemoguce je brisanje trazenog " + to_string(i) + "-tog elementa.\n";
    }
    [[nodiscard]] const char * what() const noexcept override{
        return message.c_str();
    }
};

class ECantSetData :public exception{
    string message;
public:
    explicit ECantSetData(int i): exception(){
        message = "Greska! Nemoguce je postavljanje elementa na trazenu " + to_string(i) +"-tu poziciju.\n";
    }
    [[nodiscard]] const char * what() const noexcept override{
        return message.c_str();
    }
};

class ENoColumns :public exception{
public:
    explicit ENoColumns(): exception(){}
    [[nodiscard]] const char * what() const noexcept override{
        return "Greska! Tabela ne sadrzi nijednu kolonu.\n";
    }
};

class ENotCompatibleRow :public exception{
    string message;

public:
    explicit ENotCompatibleRow(const string&  tableName): exception(){
        message = "Greska! Prosledjena vrsta nema odgovarajuci broj kolona za tabelu: " + tableName + ".\n";
    }
    [[nodiscard]] const char * what() const noexcept override{
        return message.c_str();
    }
};


class ENotCompatibleColumns :public exception{
    string message;
public:
    explicit ENotCompatibleColumns(const string& tableName): exception(){
        message = "Greska! Prosledjene kolone se ne uklapaju u kolone tabele " + tableName + ".\n";
    }
    [[nodiscard]] const char * what() const noexcept override{
        return message.c_str();
    }
};
class EAlreadyExistingTableName :public exception{
    string message;
public:
    explicit EAlreadyExistingTableName(const string& tableName): exception(){
        message = "Greska! U bazi podataka vec postoji tabela sa imenom " + tableName + ".\n";
    }
    [[nodiscard]] const char * what() const noexcept override{
        return message.c_str();
    }
};

class ENonExistingTable :public exception{
    string message;
public:
    explicit ENonExistingTable(const string& tableName): exception(){
        message = "Greska! U bazi podataka ne postoji tabela sa imenom " + tableName + ".\n";

    }
    [[nodiscard]] const char * what() const noexcept override{
        return message.c_str();
    }
};

class ENonCompatibleNumOfColsAndVals :public exception{
public:
    explicit ENonCompatibleNumOfColsAndVals(): exception(){}
    [[nodiscard]] const char * what() const noexcept override{
        return "Greska! Broj unetih kolona i vrednosti se ne poklapaju.\n";
    }
};



class ENonExistingColumn :public exception{
    string message;
public:
    explicit ENonExistingColumn(const string& columnName, const string& tableName): exception(){
        message = "Greska! Kolona: " + columnName + " ne postoji u tabeli: "+ tableName +".\n";
    }
    [[nodiscard]] const char * what() const noexcept override{
        return message.c_str();
    }
};



class EIllegalQuery :public exception{
public:
    explicit EIllegalQuery(): exception(){}
    [[nodiscard]] const char * what() const noexcept override{
        return "Greska! Uneti upit nije korektnog formata.\n";
    }
};
class EBadRowIndex :public exception{
    string message;
public:
    explicit EBadRowIndex(int index, const string& tableName): exception(){
        message = "Greska! neregularan index vrste " + to_string(index) + " u tabeli: "+ tableName +".\n";
    }
    [[nodiscard]] const char * what() const noexcept override{
        return message.c_str();
    }
};
class ENotOkNicknames :public exception{
public:
    explicit ENotOkNicknames(): exception(){}
    [[nodiscard]] const char * what() const noexcept override{
        return "Greska! Uneti nadimci za tabele ili kolone nisu korektni.\n";
    }
};

class EAmbiguousColumnName :public exception{
    string message;
public:
    explicit EAmbiguousColumnName(const string& columnName): exception(){
        message = "Greska! Kolona: " + columnName + " nije jednoznacno definisana.\n";
    }
    [[nodiscard]] const char * what() const noexcept override{
        return message.c_str();
    }
};

#endif //POOP_PROJEKAT_1_MYEXEPTIONS_H
