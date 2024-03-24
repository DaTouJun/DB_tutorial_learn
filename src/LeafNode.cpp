//
// Created by 28318 on 24-3-18.
//

#include "LeafNode.hpp"

void LeafNode::print_leaf_node() {
    uint32_t num_cells = *leaf_node_num_cells();
    std::cout << "leaf (size " << num_cells << ")" << std::endl;
    for (uint32_t i = 0; i < num_cells; i++) {
        uint32_t key = *leaf_node_key(i);
        std::cout << " - " << i << " : " << key << std::endl;   // 首先是cell编号，然后是key
    }
}
