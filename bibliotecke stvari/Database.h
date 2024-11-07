//
// Created by Miletic on 4/18/2024.
//

#ifndef POOP_PROJEKAT_1_DATABASE_H
#define POOP_PROJEKAT_1_DATABASE_H

#include <iostream>
#include "regex"
#include "fstream"
#include "Table.h"
#include "Condition.h"
#include "Equals.h"
#include "NotEquals.h"
#include "Change.h"
#include "OrderBy.h"

class Database {

public:
    bool hasTableName(const string& tableName);

    //upiti
    void createTable(const string& tableName, const vector<string>& columns);

    void insertInto(const string& tableName, const vector<string>& columns, const vector<vector<string>>& values);

    int update(const string& tableName, vector<Change>& changes,  const vector<Condition*>& conds);

    static int deleteFrom(Table *table, vector<Condition*>& conds);

    static string select(Table* table, const vector<string>& selectedColumns, const vector<Condition*>& conds, vector<OrderBy>* order = nullptr);

    void dropTable(const string& tableName);

    void showTables() const;

    string showTablesText() const;

    //glavna obrada
    string query(const string& line);

    //export import save
    void exportDBinSQL(ostream& file) const;

    void exportDBinMyFormat(ofstream& file) const ;

    static Database* importDB(istream& file);

    string saveDB(string file);



private:
    //args
    vector<Table*> tables;

    //stringovi za regex
    const static string createTableString;
    const static string dropTableString;
    const static string insertIntoString;
    const static string updateString;
    const static string deleteString;
    const static string selectString;
    const static string selectJoinString;
    const static string showTablesString;

    //parse metode
    static vector<Condition*> parseConditions(const string& strConds);
    static vector<string> parseColumns(const string& strCols);
    static vector<Change> parseChanges(const string& strChanges);
    static vector<vector<string>> parseNewValues(const string& strNewVals);
    static vector<OrderBy>* parseOrders(const string& strOrders);


    Table* getTableByName(const string& tableName);

};




#endif //POOP_PROJEKAT_1_DATABASE_H
