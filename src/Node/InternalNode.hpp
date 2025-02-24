//
// Created by 28318 on 25-2-24.
//

#ifndef INTERNALNODE_HPP
#define INTERNALNODE_HPP

#include "Node.hpp"

#include "../EnumsAndDefs.hpp"

#include <iostream>

class InternalNode : public Node {
public:
    InternalNode();

    InternalNode(void *node) : Node(node) {
    }

    void initialize_internal_node() {
        set_node_type(NodeType::INTERNAL);
        set_node_root(false);
        *internal_node_num_keys() = 0;
    }

    uint32_t *internal_node_num_keys() {
        return (uint32_t *) ((char *) node + NODE::INTERNAL::NUM_KEYS_OFFSET);
    }

    uint32_t *internal_node_right_child() {
        return (uint32_t *) ((char *) node + NODE::INTERNAL::RIGHT_CHILD_OFFSET);
    }

    uint32_t *internal_node_cell(uint32_t cell_num) {
        return (uint32_t*) ((char*) node + NODE::INTERNAL::HEADER_SIZE + cell_num * NODE::INTERNAL::CELL_SIZE);
    }

    uint32_t *internal_node_child(uint32_t child_num) {
        uint32_t num_keys = *internal_node_num_keys();
        if (child_num > num_keys) {
            std::cout << "Tried to access child_num << " << child_num << " > num_keys" << num_keys << std::endl;
            exit(EXIT_FAILURE);
        } else if (child_num == num_keys) {
            return internal_node_right_child();
        } else {
            return internal_node_cell(child_num);
        }
    }

    uint32_t *internal_node_key(uint32_t key_num) {
        return internal_node_cell(key_num) + NODE::INTERNAL::CHILD_SIZE;
    }
    uint32_t get_node_max_key() override {
        return *internal_node_key(*internal_node_num_keys() - 1);
    }
};


#endif //INTERNALNODE_HPP
