//
// Created by 28318 on 24-3-13.
//

#include "Cursor.hpp"

#include "Table.hpp"
#include "LeafNode.hpp"
#include <unistd.h>

/**
 *
 * @param table Table的引用
 * @param option 是否从头开始,否则从末尾开始
 */
Cursor::Cursor(Table &table, bool option) : table(table) {
    // 如果设置了option, 则从头开始
    page_num = table.root_page_num;
    auto root_node = LeafNode(table.pager.get_page(table.root_page_num));
    uint32_t num_cells = *root_node.leaf_node_num_cells();
    if (option) {
        // start at the beginning of the table
        cell_num = 0;
        end_of_table = (num_cells == 0);
    } else {
        // end of the table
        cell_num = num_cells;
        end_of_table = true;
    }
}

void *Cursor::cursor_value() const { // 获得指针指向的位置
    void *page = table.pager.get_page(page_num);
    return LeafNode(page).leaf_node_value(cell_num);
}

void Cursor::cursor_advance() { // 增加cursor位置
    auto leafNode = LeafNode(table.pager.get_page(page_num));
    cell_num += 1;
    if (cell_num >= *leafNode.leaf_node_num_cells()) {
        end_of_table = true;
    }
}

void Cursor::leaf_node_insert(uint32_t key,const Row &value) {
    auto leaf_node = LeafNode(table.pager.get_page(page_num));
    uint32_t num_cells = *leaf_node.leaf_node_num_cells();  // 当前叶子节点中的cell数量

    if (num_cells >= LEAF_NODE_MAX_CELLS) {
        // Node full
        std::cout << "Need to implement splititing a leaf node" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (cell_num < num_cells) {
        // make room for new cell
        for (uint32_t i = num_cells; i > cell_num; i--) {
            memcpy(leaf_node.leaf_node_cell(i),
                   leaf_node.leaf_node_cell(i - 1),
                   LEAF_NODE_CELL_SIZE);
        }
    }

    // insert new cell
    *(leaf_node.leaf_node_num_cells()) += 1;    // 增加一个单元格
    *(leaf_node.leaf_node_key(cell_num)) = key; // 写入key
    serialize_row(value, leaf_node.leaf_node_value(cell_num));  // 这里将行序列化，同时也对id序列化了(有冗余?)
}