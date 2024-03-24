//
// Created by 28318 on 24-3-13.
//

#ifndef CURSOR_HPP
#define CURSOR_HPP
#include <cstdint>

#include "Table.hpp"

/**
 * @brief 这是指针类，用于便捷操作Table
 */
class Cursor {
private:
    Table &table;
    uint32_t page_num;
    uint32_t cell_num;
    bool end_of_table;
public:
    Cursor(Table &table, bool option);
    void *cursor_value() const;
    void cursor_advance();
    void leaf_node_insert(uint32_t key, const Row &value);

    friend class DB;
};



#endif //CURSOR_HPP
