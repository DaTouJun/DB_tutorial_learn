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
        return internal_node_find(root_page_num, key);
    }
}

std::unique_ptr<Cursor> Table::internal_node_find(uint32_t page_num, uint32_t key) {
    InternalNode node{pager.get_page(page_num)};
    uint32_t num_keys = *node.internal_node_num_keys();
    uint32_t min_index = 0, max_index = num_keys;   // 子节点数量比key多一个

    while (max_index != min_index) {
        uint32_t index = (min_index + max_index) / 2;
        uint32_t key_to_right = *node.internal_node_key(index);
        if (key_to_right >= key) {
            max_index = index;
        } else {
            min_index = index + 1;
        }
    }
    uint32_t child_num = *node.internal_node_child(min_index);
    Node child = pager.get_page(child_num);
    switch (child.get_node_type()) {
        case NodeType::INTERNAL:
            return internal_node_find(child_num, key);
        case NodeType::LEAF:
            return std::make_unique<Cursor>(*this, child_num, key);
        default:
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
