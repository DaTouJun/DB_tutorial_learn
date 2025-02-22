//
// Created by 28318 on 24-3-18.
//

#ifndef TINYDB_WITHOUTTEST_LEAFNODE_H
#define TINYDB_WITHOUTTEST_LEAFNODE_H

#include "Serializer.hpp"
#include <cstdint>
#include <iostream>


/**
 * @brief 这里定义了B+树的节点，但是现在还没有实现树
 *        同时在当前节点中，一个节点对应一个Page
 */
class LeafNode {
private:
    void *node;
public:
    /**
     * @param node 用于存储节点的内存地址
     */
    explicit LeafNode(void *node) : node(node) {}

    void initialize_leaf_node() { // 初始化叶子节点
        *leaf_node_num_cells() = 0;
    }

    uint32_t *leaf_node_num_cells() {   // 获取存储在该节点中的cell数量 通过偏移得到存储NUM_CELLS的位置
        return (uint32_t *) ((char *) node + LEAF_NODE_NUM_CELLS_OFFSET);
    }

    void *leaf_node_cell(uint32_t cell_num) { // 通过偏移获得对应cell的指针
        return (char *) node + LEAF_NODE_HEADER_SIZE + cell_num * LEAF_NODE_CELL_SIZE;
    }

    uint32_t *leaf_node_key(uint32_t cell_num) { // 因为key在cell初始，因此直接将cell指针转换为key*也就是uint32_t*类型的指针
        return (uint32_t *) leaf_node_cell(cell_num);
    }

    void *leaf_node_value(uint32_t cell_num) {  // 获得存储内容位置的指针
        return (char *) leaf_node_cell(cell_num) + LEAF_NODE_VALUE_OFFSET;
    }

    /**
     * @brief 用于打印出节点和其子节点的信息
     */
    void print_leaf_node();
};


#endif //TINYDB_WITHOUTTEST_LEAFNODE_H
