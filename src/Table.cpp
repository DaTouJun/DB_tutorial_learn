#include "Table.hpp"

#include "Cursor.hpp"
#include "Node/Node.hpp"
#include "Node/InternalNode.hpp"

#include <memory>

std::unique_ptr<Cursor> Table::table_find(uint32_t key) {
    LeafNode root_node = LeafNode(pager.get_page(root_page_num));

    if (root_node.get_node_type() == NodeType::LEAF) {
        return std::make_unique<Cursor>(*this, root_page_num, key);
    } else {
        std::cout << "Need to implement searching an internal node." << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Table::create_new_root(uint32_t right_child_page_num) {
    InternalNode root = pager.get_page(root_page_num);
    Node right_child = pager.get_page(right_child_page_num);
    uint32_t left_child_page_num = pager.get_unused_page_num();
    LeafNode left_child{pager.get_page(left_child_page_num)};

    memcpy(left_child.get_node(), root.get_node(), PAGE_SIZE);
    left_child.set_node_root(false);

    root.initialize_internal_node();
    root.set_node_root(true);
    *root.internal_node_num_keys() = 1;
    *root.internal_node_child(0) = left_child_page_num;
    uint32_t left_child_max_key = left_child.get_node_max_key();
    *root.internal_node_key(0) = left_child_max_key;
    *root.internal_node_right_child() = right_child_page_num;
}
