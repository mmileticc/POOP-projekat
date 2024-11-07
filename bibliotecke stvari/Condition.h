//
// Created by Miletic on 4/30/2024.
//

#ifndef POOP_PROJEKAT_1_CONDITION_H
#define POOP_PROJEKAT_1_CONDITION_H

#include <utility>

#include "string"
using namespace std;

class Condition  {

public:

    Condition(string columnName, string value) : value(std::move(value)), columnName(std::move(columnName)) {}

    virtual bool doesItFulfillCondition(const string& val) = 0;

    virtual string getOperationSign() = 0;

    [[nodiscard]] const string &getValue() const {return value;}

    [[nodiscard]] const string &getCondsColumnName() const {return columnName;}

    [[nodiscard]] int getIndex() const;

    void setIndex(int ind);

protected:
    string value;

    string columnName;

    int index = -1;

};


#endif //POOP_PROJEKAT_1_CONDITION_H
