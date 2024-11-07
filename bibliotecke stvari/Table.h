//
// Created by Miletic on 4/18/2024.
//

#ifndef POOP_PROJEKAT_1_TABLE_H
#define POOP_PROJEKAT_1_TABLE_H
#include "vector"
#include "array"
#include "Column.h"
#include "MyExeptions.h"
#include "OrderBy.h"
class Table {
public:
    explicit Table(const string& name) {tableName = name;}

    //kopirajuci kosntruktor
    Table(const Table&) = default;

    //geter imena tabele
    [[nodiscard]] const string &getTableName() const;

    //ispis lepo formatirane tabele
    static void printTable(Table& table);

    //setovanje kolona jednom za svagda
    void setColumns(const vector<string>& newColumns);

    //broj kolona u tabeli
    int getNumOfColumns();

    //broj vrsta u tabelu
    int getNumOfRows();

    //dodavanje cele vrste u tabelu
    void addRow(const vector<string>& newRow);

    //izmena vrednosti i-toj vrsti
    void setIthRow(const vector<string>& newRow, int I);

    //brisanje i-te vrste
    void deleteIthRow(int i);

    //dohvatanje imena kolona tabele
    [[nodiscard]] vector<string>& getNamesOfColumns()const;

    //da li tabela sadrzi sve date kolone
    bool containsAllColums(const vector<string>& cols);

    //geter kolona
     vector<Column>& getColumns()  {return columns;}

    //da li sadrzi trazenu kolonu medju svojim
    bool hasColumn(const string& columnName);

    //pravljenje stringa sa zarezima i razmacima od naziva kolona za svrhe sql exporta
    [[nodiscard]] string columnsToText() const;

    //pravljenje stringa sa zarezima i razmacima od vrednosti svake vrste u svrhe sql exporta
    [[nodiscard]] string ithRowToText(int i);

    string ithRowToTextWithoutComas(int i);

    [[nodiscard]] string tableToText();

    //zamena i-tog i j-tog reda
    void swapRows(int i, int j);

    //vraca index kolone sa datim imenom
    int getColumnIndexByName(const string& colName);

    //sortira tabelu asc ili desc u zavisnosti parametra "ad" u odnosu na kolonu datu indexom
    void sortByColumn(int colIndex, OrderSort ad);

    //radi order by tabele po svim orderBy parametrima
    void orderByTable(vector<OrderBy>* orders);

    vector<string> getIthRow(int i);

protected:
    string tableName;

    vector<Column> columns;

    //polje za formatiranje ispisa tabele
    int biggestWordLength = 0;

    //dodavanje nove kolone u tabelu
    void addColumn(const string& colName);
};


#endif //POOP_PROJEKAT_1_TABLE_H
