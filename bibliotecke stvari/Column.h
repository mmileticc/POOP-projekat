//
// Created by Miletic on 4/19/2024.
//

#ifndef POOP_PROJEKAT_1_COLUMN_H
#define POOP_PROJEKAT_1_COLUMN_H

#include <string>
#include "vector"
#include <algorithm>
#include "MyExeptions.h"

using namespace std;
class Column {
public:
    explicit Column(const string& name);

    Column(const Column& ) = default;

    Column(Column&& ) = default;

    [[nodiscard]] const string &getColumnName() const;

    string & getIthRow(int i);

    [[nodiscard]] vector<string> &getValues();

    void setColumnName(const string &columnName);

    void setValues( vector<string>& vals);

    void addRow(const string& value);

    void setIthRow(const string& value, int i);

    void deleteIthRow(int i);

    int getNumOfRows();

private:
    string columnName;
    vector<string> values;
};


#endif //POOP_PROJEKAT_1_COLUMN_H
