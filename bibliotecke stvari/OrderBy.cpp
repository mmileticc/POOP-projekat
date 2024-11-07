//
// Created by Miletic on 5/5/2024.
//

#include "OrderBy.h"

#include <utility>

OrderBy::OrderBy(string columnName, OrderSort ascdesc) : columnName(std::move(columnName)), ascdesc(ascdesc) {}
const string &OrderBy::getColumnName() const {
    return columnName;
}

OrderSort OrderBy::getAscdesc() const {
    return ascdesc;
}
