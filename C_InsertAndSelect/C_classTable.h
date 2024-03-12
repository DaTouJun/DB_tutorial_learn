//
// Created by 28318 on 2023/3/9.
//

#include "C_serialize.hpp"
#include <cstdlib>

#ifndef C_CLASS_TABLE_H
#define C_CLASS_TABLE_H

// 最大页数量
#define TABLE_MAX_PAGES 100

constexpr int PAGE_SIZE = 4096; // 4K
constexpr int ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
constexpr int TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

class Table {
public:
    uint_fast32_t num_rows{0};
    void *pages[TABLE_MAX_PAGES]{};

    Table() = default;

    virtual ~Table() {
        for (auto &page: pages) {
            free(page);
        }
    }

    static void *row_slot(Table &table, const uint_fast32_t &row_num) {
        uint_fast32_t page_num = row_num / ROWS_PER_PAGE;
        void *page = table.pages[page_num];
        if (page == nullptr)
            page = table.pages[page_num] = std::malloc(PAGE_SIZE);
        uint_fast32_t row_offset = row_num % ROWS_PER_PAGE;
        uint_fast32_t byte_offset = row_offset * ROW_SIZE;
        return (char *) page + byte_offset;
    }
};


#endif //C_CLASS_TABLE_H
