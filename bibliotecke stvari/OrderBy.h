//
// Created by Miletic on 5/5/2024.
//

#ifndef POOP_PROJEKAT_1_ORDERBY_H
#define POOP_PROJEKAT_1_ORDERBY_H
#include "string"
using namespace std;
enum class OrderSort{ ASC, DESC };

class OrderBy {
public:
    OrderBy(string columnName, OrderSort ascdesc);

    const string &getColumnName() const;

    OrderSort getAscdesc() const;

private:
    string columnName;
    OrderSort ascdesc = OrderSort::ASC;
};


#endif //POOP_PROJEKAT_1_ORDERBY_H
