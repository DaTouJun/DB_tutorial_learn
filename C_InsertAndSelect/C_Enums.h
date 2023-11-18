//
// Created by 28318 on 2023/9/1.
//

#ifndef C_INSERTANDSELECT_C_ENUMS_H
#define C_INSERTANDSELECT_C_ENUMS_H

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

#endif //C_INSERTANDSELECT_C_ENUMS_H
