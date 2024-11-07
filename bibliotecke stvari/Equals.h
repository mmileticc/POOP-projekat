//
// Created by Miletic on 4/30/2024.
//

#ifndef POOP_PROJEKAT_1_EQUALS_H
#define POOP_PROJEKAT_1_EQUALS_H
#include "Condition.h"

class Equals :public Condition{
public:
    Equals(const string& colName, const string& value): Condition(colName, value){}

    bool doesItFulfillCondition(const string& val) override;

    string getOperationSign() override;


};


#endif //POOP_PROJEKAT_1_EQUALS_H
