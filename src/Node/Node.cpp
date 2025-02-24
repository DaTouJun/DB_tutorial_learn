//
// Created by 28318 on 25-2-24.
//

#include "Node.hpp"

Node::Node(void *node) : node(node) {
}

NodeType Node::get_node_type() const {
    uint8_t value = *reinterpret_cast<uint8_t *>(static_cast<char *>(node) + NODE::TYPE_OFFSET);
    return static_cast<NodeType>(value);
}

void Node::set_node_type(NodeType type) const {
    *reinterpret_cast<uint8_t *>(static_cast<char *>(node) + NODE::TYPE_OFFSET) = static_cast<uint8_t>(type);
}

void *Node::get_node() const {
    return node;
}

bool Node::is_node_root() const {
    uint8_t value = *reinterpret_cast<uint8_t *>(static_cast<char *>(node) + NODE::IS_ROOT_OFFSET);
    return value == 1;
}

void Node::set_node_root(bool is_root) const {
    *reinterpret_cast<uint8_t *>(static_cast<char *>(node) + NODE::IS_ROOT_OFFSET) = is_root ? 1 : 0;
}
