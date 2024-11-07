//
// Created by Miletic on 4/19/2024.
//

#include "Column.h"


Column::Column(const string &name) {
    columnName = name;
}


const string &Column::getColumnName() const {
    return columnName;
}

vector<string> &Column::getValues()  {
    return values;
}

string &Column::getIthRow(int i) {
    return values[i];
}

void Column::setValues(vector<string> &vals) {
    values = vals;
}

void Column::setIthRow(const string &value, int i) {
    //provera postojanja i-tog podatka
    if(i < 0 || i >= values.size()) throw ECantSetData(i);

    values[i] = value;
}

void Column::addRow(const string &value) {
    values.push_back(value);
}

void Column::deleteIthRow(int i) {
    //provera korektnosti zahteva za brisanjem
    if(i < 0 || i >= values.size()) throw ECantDelete(i);

    auto it = values.begin()+i;
    values.erase(it);
}

int Column::getNumOfRows() {
    return (int)values.size();
}

void Column::setColumnName(const string &colName) {
    Column::columnName = colName;
}

