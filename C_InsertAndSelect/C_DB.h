//
// Created by 28318 on 2023/9/1.
//
#ifndef C_INSERTANDSELECT_C_DB_H
#define C_INSERTANDSELECT_C_DB_H

#include <iostream>
#include <string>
#include <vector>

#include "C_classTable.h"
#include "C_Enums.h"

class DB {
public:
    static void printPrompt() {
        std::cout << "db > ";
    }

    class Statement {
    public:
        StatementType type{};
        Row row_to_insert;
    };

    PrepareResult prepare_statement(const std::string&input_line, Statement&statement) {
        if (!input_line.compare(0, 6, "insert")) {
            return prepare_insert(input_line, statement);
        }
        if (!input_line.compare(0, 6, "select")) {
            statement.type = StatementType::SELECT;
            return PrepareResult::SUCCESS;
        }
        return PrepareResult::UNRECOGNIZED_STATEMENT;
    }

    PrepareResult prepare_insert(std::string s, Statement&statement) {
        using std::string;
        std::vector<string> out;
        while (!s.empty()) {
            while (s.at(std::string::size_type(0)) == ' ' && !s.empty()) {
                s.erase(0, 1);
            }
            string::size_type pos{};
            while (s.at(pos) != ' ' && pos < s.length() - 1) {
                pos++;
            }
            out.push_back(pos == s.length() - 1
                              ? s.substr({}, pos + 1)
                              : s.substr({}, pos));
            s = s.substr(pos + 1, s.length());
        }
        if (out.size() < 4)
            return PrepareResult::SYNTAX_ERROR;

        string id_string = out.at(1);
        string username = out.at(2);
        string email = out.at(3);

        int id = std::stoi(id_string);
        if (id < 0) {
            return PrepareResult::NEGATIVE_ID;
        }
        if (username.length() > COLUMN_USERNAME_SIZE) {
            return PrepareResult::STRING_TOO_LONG;
        }
        if (email.length() > COLUMN_EMAIL_SIZE) {
            return PrepareResult::STRING_TOO_LONG;
        }
        statement.row_to_insert = Row(id, username, email);
        return PrepareResult::SUCCESS;
    }

    bool parse_statement(const std::string&input_line, Statement&statement) {
        switch (prepare_statement(input_line, statement)) {
            case PrepareResult::SUCCESS:
                return false;
            case PrepareResult::UNRECOGNIZED_STATEMENT:
                std::cout << "Unrecognized keyword at start of \'" <<
                        input_line << "\'." << std::endl;
                return true;
            case PrepareResult::SYNTAX_ERROR:
                std::cout << "Syntax error. Could not parse statement." << std::endl;
                return true;
            case PrepareResult::NEGATIVE_ID:
                std::cout << "ID must be positive." << std::endl;
                return true;
            case PrepareResult::STRING_TOO_LONG:
                std::cout << "String is too long." << std::endl;
                return true;
        }
        return false;
    }

    MetaCommandResult do_meta_command(const std::string&command) {
        if (command == ".exit") {
            std::cout << "Bye!" << std::endl;
            exit(EXIT_SUCCESS);
        }
        if (command == ".help") {
            return MetaCommandResult::META_COMMAND_HELP;
        }
        return MetaCommandResult::META_COMMAND_UNRECOGNIZED_COMMAND;
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
            case StatementType::INSERT:
                executeResult = execute_insert(statement, table);
                break;
            case StatementType::SELECT:
                executeResult = execute_select(statement, table);
                break;
        }

        switch (executeResult) {
            case EXECUTE_SUCCESS:
                std::cout << "Executed." << std::endl;
                break;
            case EXECUTE_TABLE_FULL:
                std::cout << "Error : Table full." << std::endl;
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
