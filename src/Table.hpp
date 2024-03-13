//
// Created by 28318 on 2023/3/9.
//

#ifndef D_TABLE
#define D_TABLE


#include "Serializer.hpp"
#include "Pager.hpp"

#include <cstdlib>

class Table final {
public:
    Pager pager;
    uint_fast32_t num_rows;
    void *pages[TABLE_MAX_PAGES]{};

    explicit Table(const char *filename) : pager(filename) {
        num_rows = pager.file_length / ROW_SIZE;
    }

    ~Table() {
        uint32_t num_full_pages = num_rows / ROWS_PER_PAGE;
        for (uint32_t i = 0; i < num_full_pages; i++) {
            if (pager.pages[i] == nullptr) {
                continue;
            }
            pager.pager_flush(i, PAGE_SIZE);
            free(pager.pages[i]);
            pager.pages[i] = nullptr;
        }

        // There may be a partial page to write to the end of the file
        uint32_t num_additional_rows = num_rows % ROWS_PER_PAGE;
        if (num_additional_rows > 0) {
            uint32_t page_num = num_full_pages;
            if (pager.pages[page_num] != nullptr) {
                pager.pager_flush(page_num,
                                  num_additional_rows * ROW_SIZE);
                free(pager.pages[page_num]);
                pager.pages[page_num] = nullptr;
            }
        }

        int result = close(pager.file_describer);
        if (result == -1) {
            std::cout << "Error closing db file" << std::endl;
            exit(EXIT_FAILURE);
        }
        for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
            void *page = pager.pages[i];
            if (page) {
                free(page);
                pager.pages[i] = nullptr;
            }
        }
    }

    void *row_slot(const uint_fast32_t &row_num) {
        uint_fast32_t page_num = row_num / ROWS_PER_PAGE;
        /*
        void* page = table.pages[page_num];
        if (page == nullptr)
            page = table.pages[page_num] = std::malloc(PAGE_SIZE);
        */
        void *page = pager.get_page(page_num);
        if (page == nullptr){
            std::cerr << "Page pointer is null" << std::endl;
            exit(EXIT_FAILURE);
        }
        uint_fast32_t row_offset = row_num % ROWS_PER_PAGE;
        uint_fast32_t byte_offset = row_offset * ROW_SIZE;
        return static_cast<char *>(page) + byte_offset;
    }

};


#endif //D_TABLE
