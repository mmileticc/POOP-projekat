//
// Created by Miletic on 4/30/2024.
//

#include "Equals.h"

bool Equals::doesItFulfillCondition(const string &val) {
    return val == value;
}

string Equals::getOperationSign() {
    return "=";
}
