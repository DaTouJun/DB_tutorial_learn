//
// Created by 28318 on 2023/3/9.
//

#include "D_Serializer.hpp"
#include <cstdlib>

#ifndef D_TABLE
#define D_TABLE

// 最大页数量
#define TABLE_MAX_PAGES 100

constexpr int PAGE_SIZE = 4096; // 4K
constexpr int ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
constexpr int TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

class Table final {
public:
    uint_fast32_t num_rows{0};
    void* pages[TABLE_MAX_PAGES]{};

    Table() = default;

    virtual ~Table() {
        for (auto&page: pages) {
            free(page);
        }
    }

    static void* row_slot(Table&table, const uint_fast32_t&row_num) {
        uint_fast32_t page_num = row_num / ROWS_PER_PAGE;
        void* page = table.pages[page_num];
        if (page == nullptr)
            page = table.pages[page_num] = std::malloc(PAGE_SIZE);
        uint_fast32_t row_offset = row_num % ROWS_PER_PAGE;
        uint_fast32_t byte_offset = row_offset * ROW_SIZE;
        return static_cast<char *>(page) + byte_offset;
    }
};


#endif //D_TABLE
