/**
 * @brief 光标来操作table也就是页
 */

#ifndef CURSOR_HPP
#define CURSOR_HPP
#include <cstdint>

#include "Table.hpp"

/**
 * @brief 这是指针类，用于便捷操作Table
 */
class Cursor {
    Table &table;       // 对应的页
    uint32_t page_num;
    uint32_t cell_num;
    bool end_of_table;
public:
    Cursor(Table &table, bool from_begin);
    Cursor(Table &table, uint32_t page_num, uint32_t key);
    void *cursor_value() const;
    void cursor_advance();
    void leaf_node_insert(uint32_t key, const Row &value);
    void leaf_node_split_and_insert(uint32_t key, const Row &value) const;

    friend class DB;
};

#endif //CURSOR_HPP
