#include "LeafNode.hpp"

#include <iostream>

void LeafNode::print_leaf_node() {
    uint32_t num_cells = *leaf_node_num_cells();
    std::cout << "leaf (size " << num_cells << ")" << std::endl;
    for (uint32_t i = 0; i < num_cells; i++) {
        uint32_t key = *leaf_node_key(i);
        std::cout << " - " << i << " : " << key << std::endl;   // 首先是cell编号，然后是key
    }
}

uint32_t * LeafNode::leaf_node_next_leaf() const {
    return reinterpret_cast<uint32_t *>(static_cast<char *>(node) + NODE::LEAF::NEXT_LEAF_OFFSET);
}
