#include "Table.hpp"
#include "Cursor.hpp"

#include <memory>

std::unique_ptr<Cursor> Table::table_find(uint32_t key) {
    LeafNode root_node = LeafNode(pager.get_page(root_page_num));

    if (root_node.get_node_type() == NodeType::LEAF) {
        return std::make_unique<Cursor>(*this, root_page_num, key);
    } else {
        std::cout << "Need to implement searching internal nodes." << std::endl;
        exit(EXIT_FAILURE);
    }
}