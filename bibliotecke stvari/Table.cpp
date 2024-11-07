//
// Created by Miletic on 4/18/2024.
//
#include <iostream>
#include <iomanip>
#include "Table.h"
using namespace std;
void Table::printTable(Table &table) {
    int n = table.biggestWordLength + 3; // Broj karaktera na koji želimo da centriramo reč
    cout << "|";
    for(const Column& col: table.columns){
        const string& rec = col.getColumnName();
        int padding = n - (int)rec.length();
        int leftPadding = padding / 2;
        int rightPadding = padding - leftPadding;
        // Ispis centrirane reči
        cout << setw(leftPadding + (int)rec.length()) << setfill(' ') << rec << setw(rightPadding) << setfill(' ') << "|";
    }
    cout << "\n" << setw((n+1)*table.getNumOfColumns()-1) << std::setfill('-') << "\n";

    for(int i = 0; i < table.getNumOfRows(); i++){
        cout << "|";
            for(Column& col: table.columns) {
                const string& rec = col.getIthRow(i);
                int padding = n - (int)rec.length();
                int leftPadding = padding / 2;
                int rightPadding = padding - leftPadding;
                // Ispis centrirane reči
                cout << setw(leftPadding + (int)rec.length()) << setfill(' ') << rec << setw(rightPadding) << setfill(' ') << "|";

            }
            cout << endl;
        }


}

const string &Table::getTableName() const {
    return tableName;
}

void Table::setColumns(const vector<string> &newColumns) {
    for(const string& newCol: newColumns){
        int l =(int)newCol.length();
        if(l > biggestWordLength) biggestWordLength = l;
        addColumn(newCol);
    }
}

void Table::addRow(const vector<string> &newRow) {
    //provera odgovarajuce velicine nove vrste koja se dodaje
    if(newRow.size() != columns.size()) throw ENotCompatibleRow(tableName);
    for(int i = 0 ; i < newRow.size(); i++){
        int l =(int)newRow[i].length();
        if(l > biggestWordLength) biggestWordLength = l;
        columns[i].addRow(newRow[i]);
    }
}

void Table::setIthRow(const vector<string> &newRow, int I) {
    //provera odgovarajuce velicine nove vrste koja se dodaje
    if(newRow.size() != columns.size()) throw ENotCompatibleRow(tableName);
    for(int i = 0 ; i < newRow.size(); i++){
        int l =(int)newRow[i].length();
        if(l > biggestWordLength) biggestWordLength = l;
        columns[i].setIthRow(newRow[i], I);
    }
}

vector<string> &Table::getNamesOfColumns() const {
    auto *colNames = new vector<string>;
    for(const Column& c: columns){
        colNames->push_back(c.getColumnName());
    }
    return *colNames;
}

int Table::getNumOfRows() {
    if(!getNumOfColumns()) throw ENoColumns();
    return columns[0].getNumOfRows();
}

int Table::getNumOfColumns() {
    return (int)columns.size();
}

bool Table::hasColumn(const string &columnName) {
    return std::any_of(columns.begin(), columns.end(), [&columnName](const Column& t) {
        return t.getColumnName() == columnName;
    });
}

bool Table::containsAllColums(const vector<string> &cols) {
    return std::all_of(cols.begin(), cols.end(), [this](const std::string& col) { return hasColumn(col); });
}

string Table::columnsToText() const {
    string rez;
    for(int i = 0; i < columns.size(); i++){
        rez += columns[i].getColumnName();
        if(i!= columns.size()-1) rez += ", ";
    }
    return rez;
}

string Table::ithRowToText(int i)  {
    string rez;
    for(int j = 0; j < columns.size(); j++){
        rez +="\'"+ columns[j].getValues()[i] + "\'";   //ovde mozda budu trebali da idu dupli navodnici
        if(j!= columns.size()-1) rez += ", ";
    }
    return rez;
}
string Table::ithRowToTextWithoutComas(int i)  {
    string rez;
    for(int j = 0; j < columns.size(); j++){
        rez += columns[j].getValues()[i];   //ovde mozda budu trebali da idu dupli navodnici
        if(j!= columns.size()-1) rez += ",";
    }
    return rez;
}

string Table::tableToText() {

    string rez = "Table\n";
    rez += columnsToText();
    rez += "\n";
    for(int i = 0; i < getNumOfRows(); i++){
        rez += ithRowToTextWithoutComas(i);
        if(i != getNumOfRows()-1) rez += "\n";
    }
    return rez;
}

void Table::deleteIthRow(int i) {
    for(Column& c : columns){
        c.deleteIthRow(i);
    }
}

void Table::addColumn(const string &colName) {
    columns.emplace_back(colName);
}

int Table::getColumnIndexByName(const string &colName) {
    auto it = find_if(getColumns().begin(), getColumns().end(), [colName](const Column& obj) {
        return obj.getColumnName() == colName;
    });
    if (it != getColumns().end()) {
        // element je pronadjen
        return distance(getColumns().begin(), it);
    }
    else return -1;

}

void Table::swapRows(int i, int j) {
    if (i < 0 || i > getNumOfRows() - 1) throw EBadRowIndex(i, tableName);
    if (j < 0 || j > getNumOfRows() - 1) throw EBadRowIndex(j, tableName);
    if(i == j) return;
    int n = getNumOfColumns();
    for(int k = 0; k < n; k++){
        swap(getColumns()[k].getValues()[i], getColumns()[k].getValues()[j]);
        /*string t = getColumns()[k].getValues()[i];
        getColumns()[k].getValues()[i] = getColumns()[k].getValues()[j];
        getColumns()[k].getValues()[j] = t;*/
    }
}

void Table::orderByTable(vector<OrderBy> *orders) {
    for(auto o = orders->rbegin(); o != orders->rend(); ++o ){
        int index = getColumnIndexByName((*o).getColumnName());
        sortByColumn(index, (*o).getAscdesc());
    }
}

void Table::sortByColumn(int colIndex, OrderSort ad) {
    for(int i = 0; i < getNumOfRows(); i++){
        for(int j = 0; j < getNumOfRows(); j++){
            if( getColumns()[colIndex].getValues()[i].compare(getColumns()[colIndex].getValues()[j]) < 0 && ad == OrderSort::ASC){
                swapRows(i, j);
            }
            if( getColumns()[colIndex].getValues()[i].compare(getColumns()[colIndex].getValues()[j]) > 0 && ad == OrderSort::DESC){
                swapRows(i, j);
            }
        }
    }
}

vector<string> Table::getIthRow(int i) {
    vector<string> vec;
    for(auto c: columns){
        vec.push_back(c.getValues()[i]);
    }
    return vec;
}

