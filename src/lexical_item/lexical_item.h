#ifndef LEXICAL_ITEM_H
#define LEXICAL_ITEM_H

#include <iostream>

class lexical_item {
public:
    virtual ~lexical_item() {}
    virtual void print(std::ostream& out) const = 0;
};

#endif // LEXICAL_ITEM_H
