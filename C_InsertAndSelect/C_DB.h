//
// Created by 28318 on 2023/9/1.
//
#include <iostream>
#include <string>
#include "C_classTable.h"
#include "C_Enums.h"

#ifndef C_INSERTANDSELECT_C_DB_H
#define C_INSERTANDSELECT_C_DB_H


class DB {
public:
    static void printPrompt() {
        std::cout << "db > ";
    }

    class Statement {
    public:
        StatementType type;
        Row row_to_insert;
    };

    PrepareResult prepare_statement(const std::string&input_line, Statement&statement) {
        if (!input_line.compare(0, 6, "insert")) {
            statement.type = STATEMENT_INSERT;
            int args_assigned = sscanf(
                input_line.c_str(), "insert %d %s %s",
                &(statement.row_to_insert.id),
                statement.row_to_insert.username,
                statement.row_to_insert.email
            );
            if (args_assigned < 3)
                return PrepareResult::PREPARE_SYNTAX_ERROR;
            return PrepareResult::PREPARE_SUCCESS;
        }
        if (!input_line.compare(0, 6, "select")) {
            statement.type = STATEMENT_SELECT;
            return PrepareResult::PREPARE_SUCCESS;
        }
        return PrepareResult::PREPARE_UNRECOGNIZED_STATEMENT;
    }

    bool parse_statement(const std::string&input_line, Statement&statement) {
        switch (prepare_statement(input_line, statement)) {
            case PREPARE_SUCCESS:
                return false;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                std::cout << "Unrecognized keyword at start of \'" <<
                        input_line << "\'." << std::endl;
                return true;
            case PREPARE_SYNTAX_ERROR:
                std::cout << "Syntax error. Could not parse statement." << std::endl;
                return true;
        }
        return false;
    }

    MetaCommandResult do_meta_command(const std::string&command) {
        if (command == ".exit") {
            std::cout << "Bye!" << std::endl;
            exit(EXIT_SUCCESS);
        }
        else if (command == ".help") {
            return MetaCommandResult::META_COMMAND_HELP;
        }
        else {
            return MetaCommandResult::META_COMMAND_UNRECOGNIZED_COMMAND;
        }
    }

    bool parse_meta_command(const std::string&command) {
        if (command[0] == '.') {
            switch (DB::do_meta_command(command)) {
                case META_COMMAND_HELP:
                    std::cout << ".help : 打印帮助" << std::endl;
                    std::cout << ".exit : 退出" << std::endl;
                    return true;
                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    std::cout << "Unrecognized command:" << command << std::endl;
                    return true;
                case META_COMMAND_SUCCESS:
                    return true;
            }
        }
        return false;
    }

    ExecuteResult execute_insert(Statement&statement, Table&table) {
        if (table.num_rows >= TABLE_MAX_ROWS) {
            std::cerr << "Error : Table full." << std::endl;
            return EXECUTE_TABLE_FULL;
        }
        void* page = Table::row_slot(table, table.num_rows);
        serialize_row(statement.row_to_insert, page);
        table.num_rows++;

        return ExecuteResult::EXECUTE_SUCCESS;
    }

    ExecuteResult execute_select(Statement&statement, Table&table) {
        for (uint32_t i = 0; i < table.num_rows; i++) {
            Row row;
            void* page = Table::row_slot(table, i);
            deserialize_row(page, row);
            std::cout << "(" << row.id << ", " << row.username << ", " << row.email << ")" << std::endl;
        }
        return EXECUTE_SUCCESS;
    }

    void execute_statement(Statement&statement, Table&table) {
        ExecuteResult executeResult;
        switch (statement.type) {
            case STATEMENT_INSERT:
                executeResult = execute_insert(statement, table);
                break;
            case STATEMENT_SELECT:
                executeResult = execute_select(statement, table);
                break;
        }

        switch (executeResult) {
            case EXECUTE_SUCCESS:
                std::cout << "Executed." << std::endl;
                break;
            case EXECUTE_TABLE_FULL:
                std::cout << "Error: Table full." << std::endl;
                break;
        }
    }


    [[noreturn]] void start() {
        Table table;

        while (true) {
            DB::printPrompt();

            std::string input_line;
            std::getline(std::cin, input_line); // getline获取一行的输入

            if (parse_meta_command(input_line))
                continue;

            Statement statement{};
            if (parse_statement(input_line, statement))
                continue;

            execute_statement(statement, table);
        }
    }
}; // DB_start

#endif //C_INSERTANDSELECT_C_DB_H
