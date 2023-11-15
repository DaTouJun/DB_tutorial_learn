// Copyright (c) DaTouJun 2022 11
// SetForGroup MCOTF

//
// Created by DaTouJun on 2022/11/4.
//

#include <iostream>
#include <string>

#define EXIT_SUCCESS 0

class DB {
public:
    enum MetaCommandResult {
        META_COMMAND_SUCCESS,
        META_COMMAND_UNRECOGNIZED_COMMAND,
        META_COMMAND_HELP
    };
    enum PrepareResult {
        PREPARE_SUCCESS,
        PREPARE_UNRECOGNIZED_STATEMENT
    };
    enum StatementType {
        STATEMENT_INSERT,
        STATEMENT_SELECT
    };

    static void printPrompt() {
        std::cout << "db-> ";
    }

    class Statement {
    public:
        StatementType type;
    };

    PrepareResult prepare_statement(const std::string &input_line, Statement &statement) {
        if (!input_line.compare(0, 6, "insert")) {
            statement.type = STATEMENT_INSERT;
            return PrepareResult::PREPARE_SUCCESS;
        } else if (!input_line.compare(0, 6, "select")) {
            statement.type = STATEMENT_SELECT;
            return PrepareResult::PREPARE_SUCCESS;
        } else {
            return PrepareResult::PREPARE_UNRECOGNIZED_STATEMENT;
        }
    }

    bool parse_statement(const std::string &input_line, Statement &statement) {
        switch (prepare_statement(input_line, statement)) {
            case PREPARE_SUCCESS:
                return false;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                std::cout << "Unrecognized keyword at start of \'" <<
                          input_line << "\'" << std::endl;
                return true;
        }
        return false;
    }

    MetaCommandResult do_meta_command(const std::string &command) {
        if (command == ".exit") {
            std::cout << "Bye!" << std::endl;
            exit(EXIT_SUCCESS);
        } else if (command == ".help") {
            return MetaCommandResult::META_COMMAND_HELP;
        } else {
            return MetaCommandResult::META_COMMAND_UNRECOGNIZED_COMMAND;
        }
    }

    bool parse_meta_command(const std::string &command) {
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

    void execute_statement(Statement &statement) {
        switch (statement.type) {
            case STATEMENT_INSERT:
                std::cout << "Executing insert statement" << std::endl;
                break;
            case STATEMENT_SELECT:
                std::cout << "Executing select statement" << std::endl;
                break;
        }
    }

    [[noreturn]] void start() {
        while (true) {
            DB::printPrompt();

            std::string input_line;
            std::getline(std::cin, input_line);     // getline获取一行的输入

            if (parse_meta_command(input_line)) {
                continue;
            }
            Statement statement{};
            if (parse_statement(input_line, statement))
                continue;
            execute_statement(statement);
        }
    }

}; // DB_start


int main() {
    DB db;
    db.start();
}