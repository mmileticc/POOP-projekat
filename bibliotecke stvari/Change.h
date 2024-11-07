//
// Created by Miletic on 4/30/2024.
//

#ifndef POOP_PROJEKAT_1_CHANGE_H
#define POOP_PROJEKAT_1_CHANGE_H

#include "string"
using namespace  std;
class Change {

public:
    Change(string column, string newValue);

    [[nodiscard]] const string &getChangeColumnName() const;

    [[nodiscard]] const string &getNewValue() const;

    [[nodiscard]] int getIndex() const;

    void setIndex(int index);


private:
    string column;
    string newValue;
    int index = -1;
};



#endif //POOP_PROJEKAT_1_CHANGE_H
