//
// Created by Miletic on 4/30/2024.
//

#include "Change.h"

#include <utility>

Change::Change(string column, string newValue) : column(std::move(column)), newValue(std::move(newValue)) {}


const string &Change::getChangeColumnName() const {
    return column;
}

const string &Change::getNewValue() const {
    return newValue;
}

int Change::getIndex() const {
    return index;
}

void Change::setIndex(int ind) {
    Change::index = ind;
}