//
// Created by Miletic on 4/30/2024.
//

#ifndef POOP_PROJEKAT_1_NOTEQUALS_H
#define POOP_PROJEKAT_1_NOTEQUALS_H
#include "Condition.h"

class NotEquals : public Condition{
public:
    NotEquals(const string& colName, const string& value): Condition(colName, value){}

    bool doesItFulfillCondition(const string& val) override{
        return val != value;
    }

    string getOperationSign() override {return "<>";}
};


#endif //POOP_PROJEKAT_1_NOTEQUALS_H
