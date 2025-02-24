//
// Created by 28318 on 24-3-18.
//

#ifndef TINYDB_WITHOUTTEST_LEAFNODE_H
#define TINYDB_WITHOUTTEST_LEAFNODE_H

#include "../Serializer.hpp"
#include "../EnumsAndDefs.hpp"
#include "Node.hpp"
#include <cstdint>

/**
 * @brief 这里定义了B+树的节点，但是现在还没有实现树
 *        同时在当前节点中，一个节点对应一个Page
 */
class LeafNode : public Node {
public:
    LeafNode() {
    }

    explicit LeafNode(void *node) : Node(node) {
    }

    void initialize_leaf_node() {
        // 初始化叶子节点
        set_node_type(NodeType::LEAF);
        set_node_root(false);
        *leaf_node_num_cells() = 0;
    }

    uint32_t *leaf_node_num_cells() const {
        // 获取存储在该节点中的cell数量 通过偏移得到存储NUM_CELLS的位置
        return reinterpret_cast<NODE::LEAF::NUM_CELLS_TYPE *>(static_cast<char *>(node) + NODE::LEAF::NUM_CELLS_OFFSET);
    }

    void *leaf_node_cell(uint32_t cell_num) const {
        // 通过偏移获得对应cell的指针
        return static_cast<char *>(node) + NODE::LEAF::HEADER_SIZE + cell_num * NODE::LEAF::CELL_SIZE;
    }

    uint32_t *leaf_node_key(uint32_t cell_num) {
        // 因为key在cell初始，因此直接将cell指针转换为key*也就是uint32_t*类型的指针
        return (uint32_t *) leaf_node_cell(cell_num);
    }

    void *leaf_node_value(uint32_t cell_num) {
        // 获得存储内容位置的指针
        return (char *) leaf_node_cell(cell_num) + NODE::LEAF::VALUE_OFFSET;
    }

    /**
     * @brief 用于打印出节点和其子节点的信息
     */
    void print_leaf_node();

    uint32_t get_node_max_key() override {
        return *leaf_node_key(*leaf_node_num_cells() - 1);
    }
};


#endif //TINYDB_WITHOUTTEST_LEAFNODE_H
