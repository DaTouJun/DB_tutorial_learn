//
// Created by 28318 on 25-2-24.
//

#ifndef NODE_HPP
#define NODE_HPP
#include <cstdint>

#include "../EnumsAndDefs.hpp"

class Node {
protected:
    void *node;

public:
    Node() {
        node = nullptr;
    };

    Node(void *node);

    virtual ~Node() {
    }

    NodeType get_node_type() const;

    void set_node_type(NodeType type) const;

    void *get_node() const;

    bool is_node_root() const;

    void set_node_root(bool is_root) const;

    virtual uint32_t get_node_max_key() { return -1; };
};

#endif //NODE_HPP
