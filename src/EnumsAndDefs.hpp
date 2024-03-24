//
// Created by 28318 on 2023/9/1.
//

#ifndef D_ENUMS
#define D_ENUMS

enum class MetaCommandResult {
    SUCCESS,
    UNRECOGNIZED_COMMAND,
    HELP
};

enum class PrepareResult {
    SUCCESS,
    UNRECOGNIZED_STATEMENT,
    SYNTAX_ERROR,
    NEGATIVE_ID,
    STRING_TOO_LONG
};

enum class StatementType {
    INSERT,
    SELECT
};

enum class ExecuteResult {
    SUCCESS,
    TABLE_FULL,
    ERROR
};

enum class NodeTypes{
    INTERNAL,
    LEAF
};



// ------------------ Serializers ---------------------------------
typedef uint32_t KEY_TYPE;
#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)
constexpr int ID_SIZE = size_of_attribute(Row, id);
constexpr int USERNAME_SIZE = size_of_attribute(Row, username);
constexpr int EMAIL_SIZE = size_of_attribute(Row, email);

constexpr int ID_OFFSET = 0;
constexpr int USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
constexpr int EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;

constexpr int ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

constexpr int PAGE_SIZE = 4096; // 4K
constexpr int ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
constexpr int TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

// ----------------- Tree Nodes ------------------------------------
// Common Node layout 简单节点的结构
constexpr uint32_t NODE_TYPE_SIZE = sizeof(uint8_t);
constexpr uint32_t NODE_TYPE_OFFSET = 0;
constexpr uint32_t IS_ROOT_SIZE = sizeof(uint8_t);
constexpr uint32_t IS_ROOT_OFFSET = NODE_TYPE_SIZE;
constexpr uint32_t PARENT_POINTER_SIZE = sizeof(uint32_t);
constexpr uint32_t PARENT_POINTER_OFFSET = IS_ROOT_OFFSET + IS_ROOT_SIZE;
constexpr uint8_t COMMON_NODE_HEADER_SIZE =
        NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;

// Leaf Node Header layout 叶子节点的结构
typedef uint32_t LEAF_NODE_NUM_CELLS_TYPE;
constexpr uint32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(LEAF_NODE_NUM_CELLS_TYPE); // 节点用于存储
constexpr uint32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;
constexpr uint32_t LEAF_NODE_HEADER_SIZE =
        COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE;

// Leaf node body layout 叶子节点体的结构
constexpr uint32_t LEAF_NODE_KEY_SIZE = sizeof(KEY_TYPE);
constexpr uint32_t LEAF_NODE_KEY_OFFSET = 0;
constexpr uint32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE;
constexpr uint32_t LEAF_NODE_VALUE_OFFSET =
        LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE;
constexpr uint32_t LEAF_NODE_CELL_SIZE = LEAF_NODE_VALUE_OFFSET + LEAF_NODE_VALUE_SIZE;

// Cells
constexpr uint32_t LEAF_NODE_SPACE_FOR_CELLS = PAGE_SIZE - LEAF_NODE_HEADER_SIZE;
constexpr uint32_t LEAF_NODE_MAX_CELLS =
        LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE;

#endif //D_ENUMS
