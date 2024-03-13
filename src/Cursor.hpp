//
// Created by 28318 on 24-3-13.
//

#ifndef CURSOR_HPP
#define CURSOR_HPP
#include <cstdint>

#include "Table.hpp"

class Cursor {
public:
    Table &table;
    uint32_t row_num;
    bool end_of_table;

    Cursor(Table &table, bool option);
    void *cursor_value() const;
    void cursor_advance();
};



#endif //CURSOR_HPP
