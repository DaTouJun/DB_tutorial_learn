#include "Pager.hpp"

#include "Node/InternalNode.hpp"
#include "Node/LeafNode.hpp"
#include "Node/Node.hpp"

void Pager::print_tree(uint32_t page_num, uint32_t indentation_level) {
    auto node = std::make_unique<Node>(get_page(page_num));
    uint32_t num_keys, child;
    switch (node->get_node_type()) {
        case NodeType::LEAF:
            num_keys = *(static_cast<LeafNode *>(node.get())->leaf_node_num_cells());
            indent(indentation_level);
            std::cout << " - leaf (size " << num_keys << ")" << std::endl;
            for (uint32_t i = 0; i < num_keys; i++) {
                indent(indentation_level + 1);
                std::cout << " - " << *(static_cast<LeafNode *>(node.get())->leaf_node_key(i)) << std::endl;
            }
            break;
        case NodeType::INTERNAL:
            num_keys = *static_cast<InternalNode *>(node.get())->internal_node_num_keys();
            indent(indentation_level);
            std::cout << " - internal (size " << num_keys << ")" << std::endl;
            for (uint32_t i = 0; i < num_keys; i++) {
                child = *static_cast<InternalNode *>(node.get())->internal_node_child(i);
                print_tree(child, indentation_level + 1);

                indent(indentation_level + 1);
                std::cout << " - key " << *(static_cast<InternalNode *>(node.get())->internal_node_key(i)) << std::endl;
            }
            child = *static_cast<InternalNode *>(node.get())->internal_node_right_child();
            print_tree(child, indentation_level + 1);
            break;
        default:
            std::cerr << "Unknown node type" << std::endl;
            exit(EXIT_FAILURE);
    }
}
