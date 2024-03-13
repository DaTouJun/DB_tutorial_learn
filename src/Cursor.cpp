//
// Created by 28318 on 24-3-13.
//

#include "Cursor.hpp"

#include "Table.hpp"
#include <unistd.h>

Cursor::Cursor(Table& table, bool option) : table(table) {
    if (option) {
        // start at the beginning of the table
        row_num = 0;
        end_of_table = (table.num_rows == 0);
    }
    else {
        // end of the table
        row_num = table.num_rows;
        end_of_table = false;
    }
}

void* Cursor::cursor_value() const {
    const uint32_t page_num = row_num / ROWS_PER_PAGE;
    void* page = table.pager.get_page(page_num);
    const uint32_t row_offset = row_num % ROWS_PER_PAGE;
    const uint32_t byte_offset = row_offset * ROW_SIZE;
    return static_cast<char*>(page) + byte_offset;
}

void Cursor::cursor_advance() {
    row_num += 1;
    if (row_num >= table.num_rows) {
        end_of_table = true;
    }
}
