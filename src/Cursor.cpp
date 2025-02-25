//
// Created by 28318 on 24-3-13.
//

#include "Cursor.hpp"

#include "Table.hpp"
#include "Node/LeafNode.hpp"
#include <unistd.h>

/**
 *
 * @param table Table的引用
 * @param from_begin 是否从头开始,否则从末尾开始
 */
Cursor::Cursor(Table &table, bool from_begin) : table(table) {
    // 如果设置了option, 则从头开始
    page_num = table.root_page_num;
    auto root_node = LeafNode(table.pager.get_page(table.root_page_num));
    uint32_t num_cells = *root_node.leaf_node_num_cells();
    if (from_begin) {
        // start at the beginning of the table
        cell_num = 0;
        end_of_table = (num_cells == 0);
    } else {
        // end of the table
        cell_num = num_cells;
        end_of_table = true;
    }
}

Cursor::Cursor(Table &table, uint32_t page_num, uint32_t key)
    : table(table),
      page_num(page_num) {
    this->end_of_table = false;
    this->page_num = page_num;
    this->end_of_table = false;

    LeafNode root_node{table.pager.get_page(page_num)};
    uint32_t num_cells = *root_node.leaf_node_num_cells();

    uint32_t min_index = 0;
    uint32_t one_past_max_index = num_cells;
    while (one_past_max_index != min_index) {
        uint32_t index = (min_index + one_past_max_index) / 2;
        uint32_t key_at_index = *root_node.leaf_node_key(index);

        if (key == key_at_index) {
            this->cell_num = index;
            return;
        } else if (key < key_at_index) {
            one_past_max_index = index;
        } else {
            min_index = index + 1;
        }
    }
    this->cell_num = min_index;
}


void *Cursor::cursor_value() const {
    // 获得指针指向的位置
    void *page = table.pager.get_page(page_num);
    return LeafNode(page).leaf_node_value(cell_num);
}

void Cursor::cursor_advance() {
    // 增加cursor位置
    auto leafNode = LeafNode(table.pager.get_page(page_num));
    cell_num += 1;
    if (cell_num >= *leafNode.leaf_node_num_cells()) {
        end_of_table = true;
    }
}

void Cursor::leaf_node_insert(uint32_t key, const Row &value) {
    auto leaf_node = LeafNode(table.pager.get_page(page_num));
    uint32_t num_cells = *leaf_node.leaf_node_num_cells(); // 当前叶子节点中的cell数量

    if (num_cells >= NODE::LEAF::MAX_CELLS) {
        // Node full
        leaf_node_split_and_insert(key, value);
        return;
    }

    if (cell_num < num_cells) {
        // make room for new cell
        for (uint32_t i = num_cells; i > cell_num; i--) {
            memcpy(leaf_node.leaf_node_cell(i),
                   leaf_node.leaf_node_cell(i - 1),
                   NODE::LEAF::CELL_SIZE);
        }
    }

    // insert new cell
    *(leaf_node.leaf_node_num_cells()) += 1; // 增加一个单元格
    *(leaf_node.leaf_node_key(cell_num)) = key; // 写入key
    serialize_row(value, leaf_node.leaf_node_value(cell_num)); // 这里将行序列化，同时也对id序列化了(有冗余?)
}

void Cursor::leaf_node_split_and_insert(uint32_t key, const Row &value) {
    LeafNode old_node{table.pager.get_page(page_num)};
    uint32_t new_page_num = table.pager.get_unused_page_num();
    LeafNode new_node{table.pager.get_page(new_page_num)};
    new_node.initialize_leaf_node();

    for (int32_t i = NODE::LEAF::MAX_CELLS; i >= 0; i--) {
        LeafNode *destination_node;

        if (i >= NODE::LEAF::LEFT_SPLIT_COUNT) {
            destination_node = &new_node;
        } else {
            destination_node = &old_node;
        }

        uint32_t index_within_node = i % NODE::LEAF::LEFT_SPLIT_COUNT;
        void *destination = destination_node->leaf_node_cell(index_within_node);

        if (i == cell_num) {
            serialize_row(value, destination);
        } else if (i > cell_num) {
            memcpy(destination, old_node.leaf_node_cell(i - 1), NODE::LEAF::CELL_SIZE);
        } else {
            memcpy(destination, old_node.leaf_node_cell(i), NODE::LEAF::CELL_SIZE);
        }
    }

    *old_node.leaf_node_num_cells() = NODE::LEAF::LEFT_SPLIT_COUNT;
    *new_node.leaf_node_num_cells() = NODE::LEAF::RIGHT_SPLIT_COUNT;

    if (old_node.is_node_root()) {
        return table.create_new_root(new_page_num);
    } else {
        std::cout << "Need to implement updating parent after split" << std::endl;
        exit(EXIT_FAILURE);
    }
}
