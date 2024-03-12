//
// Created by 28318 on 2023/9/1.
//
#ifndef D_DB
#define D_DB

#include <iostream>
#include <memory>

#include "Table.hpp"
#include "Enums.hpp"

class DB {
private:
    std::unique_ptr<Table> table;
public:
    explicit DB(const char *filename) {
        table = std::make_unique<Table>(filename);
    }

    static void printPrompt() {
        std::cout << "db > ";
    }

    class Statement {
    public:
        StatementType type{};
        Row row_to_insert;
    };

    [[noreturn]] void start();

    bool parse_meta_command(const std::string &command);

    MetaCommandResult do_meta_command(const std::string &command);

    bool parse_statement(const std::string &input_line, Statement &statement);

    static PrepareResult prepare_statement(const std::string &input_line, Statement &statement);

    static PrepareResult prepare_insert(std::string s, Statement &statement);

    void execute_statement(const Statement &statement);

    ExecuteResult execute_insert(const Statement &statement);

    ExecuteResult execute_select();
}; // DB_start

#endif //D_DB
