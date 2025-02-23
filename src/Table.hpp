/**
 * @class Table 页对象
 * @brief
 */

#ifndef D_TABLE
#define D_TABLE

#include "Serializer.hpp"
#include "Pager.hpp"
#include "LeafNode.hpp"

#include <cstdlib>
#include <memory>

class Cursor;

class Table final {
private:
    uint32_t root_page_num;     // 用于存储根节点的页面号  现在一个节点对应一个page
    Pager pager;
    uint_fast32_t num_rows;
public:
    // 初始化 计算已有行数
    explicit Table(const char *filename) : pager(filename) {
        num_rows = pager.file_length / ROW_SIZE; // 已经有的行数
        root_page_num = 0;  // 现在就一个page
        if (pager.num_pages == 0) { // 目前页面没有节点，要创建
            LeafNode root_node = LeafNode(pager.get_page(0)); // 获得第0页，同时 pager自动创建对应文件
            root_node.initialize_leaf_node();
        }
    }

    ~Table() {  // 在Table销毁时 开始执行保存操作
        for (uint32_t i = 0; i < pager.num_pages; i++) {
            if (pager.pages[i] == nullptr) {
                continue;
            }
            pager.pager_flush(i);
            free(pager.pages[i]);
            pager.pages[i] = nullptr;
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

    // 根据行号获得行对应指针
    void *row_slot(const uint_fast32_t &row_num) {
        uint_fast32_t page_num = row_num / ROWS_PER_PAGE;
        void *page = pager.get_page(page_num);
        if (page == nullptr) {
            std::cerr << "Page pointer is null" << std::endl;
            exit(EXIT_FAILURE);
        }
        uint_fast32_t row_offset = row_num % ROWS_PER_PAGE;
        uint_fast32_t byte_offset = row_offset * ROW_SIZE;
        return static_cast<char *>(page) + byte_offset;
    }

    std::unique_ptr<Cursor> table_find(uint32_t key);

    friend class Cursor;

    friend class DB;
};

#endif //D_TABLE
