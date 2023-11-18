//
// Created by 28318 on 2023/9/1.
//
#ifndef D_DB
#define D_DB

#include <iostream>

#include "D_Table.hpp"
#include "D_Enums.hpp"

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

    [[noreturn]] static void start();

    static bool parse_meta_command(const std::string&command);

    static MetaCommandResult do_meta_command(const std::string&command);

    static bool parse_statement(const std::string&input_line, Statement&statement);

    static PrepareResult prepare_statement(const std::string&input_line, Statement&statement);

    static PrepareResult prepare_insert(std::string s, Statement&statement);

    static void execute_statement(const Statement&statement, Table&table);

    static ExecuteResult execute_insert(const Statement&statement, Table&table);

    static ExecuteResult execute_select(Table&table);
}; // DB_start

#endif //D_DB
