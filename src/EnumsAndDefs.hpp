//
// Created by 28318 on 2023/9/1.
//

#ifndef ENUMS_AND_DEFINES
#define ENUMS_AND_DEFINES

#include "Row.hpp"

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
    DUPLICATE_KEY,
    ERROR
};

/// @brief 节点类型
enum class NodeType {
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

namespace NODE {
    constexpr uint32_t TYPE_OFFSET = 0;
    constexpr uint32_t TYPE_SIZE = sizeof(uint8_t);

    constexpr uint32_t IS_ROOT_OFFSET = TYPE_SIZE + TYPE_OFFSET;
    constexpr uint32_t IS_ROOT_SIZE = sizeof(uint8_t);

    constexpr uint32_t PARENT_POINTER_OFFSET = IS_ROOT_SIZE + IS_ROOT_OFFSET;
    constexpr uint32_t PARENT_POINTER_SIZE = sizeof(uint32_t);

    // 节点头通用大小
    constexpr uint32_t COMMON_NODE_HEADER_SIZE = PARENT_POINTER_SIZE + PARENT_POINTER_OFFSET;

    namespace LEAF {
        typedef uint32_t NUM_CELLS_TYPE;
        constexpr uint32_t NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;
        constexpr uint32_t NUM_CELLS_SIZE = sizeof(NUM_CELLS_TYPE);
        constexpr uint32_t HEADER_SIZE = NUM_CELLS_SIZE + NUM_CELLS_OFFSET;

        // 内容
        constexpr uint32_t KEY_SIZE = sizeof(KEY_TYPE); // 重复
        constexpr uint32_t KEY_OFFSET = 0;

        constexpr uint32_t VALUE_SIZE = ROW_SIZE;
        constexpr uint32_t VALUE_OFFSET = KEY_SIZE + KEY_OFFSET;

        constexpr uint32_t CELL_SIZE = VALUE_SIZE  + VALUE_OFFSET;

        // cells
        constexpr uint32_t SPACE_FOR_CELLS = PAGE_SIZE - HEADER_SIZE;
        constexpr uint32_t MAX_CELLS = SPACE_FOR_CELLS / CELL_SIZE;

        // split
        constexpr uint32_t RIGHT_SPLIT_COUNT = (MAX_CELLS + 1) / 2;
        constexpr uint32_t LEFT_SPLIT_COUNT = (MAX_CELLS + 1) - RIGHT_SPLIT_COUNT;
    }

    namespace INTERNAL {
        constexpr uint32_t NUM_KEYS_OFFSET = COMMON_NODE_HEADER_SIZE;
        constexpr uint32_t NUM_KEYS_SIZE = sizeof(uint32_t);

        constexpr uint32_t RIGHT_CHILD_OFFSET = NUM_KEYS_OFFSET + NUM_KEYS_SIZE;    // 连接右侧节点
        constexpr uint32_t RIGHT_CHILD_SIZE = sizeof(uint32_t);

        constexpr uint32_t HEADER_SIZE = RIGHT_CHILD_OFFSET + RIGHT_CHILD_SIZE;

        // 内容
        constexpr uint32_t KEY_SIZE = sizeof(KEY_TYPE);
        constexpr uint32_t CHILD_SIZE = sizeof(KEY_TYPE);
        constexpr uint32_t CELL_SIZE = KEY_SIZE + CHILD_SIZE;
    }
}

#endif //ENUMS_AND_DEFINES
