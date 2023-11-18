//
// Created by 28318 on 2023/9/1.
//

#ifndef C_INSERTANDSELECT_C_ENUMS_H
#define C_INSERTANDSELECT_C_ENUMS_H

enum MetaCommandResult {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND,
    META_COMMAND_HELP
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

enum ExecuteResult {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL
};

#endif //C_INSERTANDSELECT_C_ENUMS_H
