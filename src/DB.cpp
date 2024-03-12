//
// Created by 28318 on 2023/11/18.
//
#include "DB.hpp"
#include <vector>
#include <string>
#include <memory>



void DB::start() {
    while (true) {
        printPrompt();

        std::string input_line;
        std::getline(std::cin, input_line); // getline获取一行的输入

        if (parse_meta_command(input_line))
            continue;

        Statement statement{};
        if (parse_statement(input_line, statement))
            continue;

        execute_statement(statement);
    }
}

PrepareResult DB::prepare_statement(const std::string &input_line, Statement &statement) {
    if (!input_line.compare(0, 6, "insert")) {
        return prepare_insert(input_line, statement);
    }
    if (!input_line.compare(0, 6, "select")) {
        statement.type = StatementType::SELECT;
        return PrepareResult::SUCCESS;
    }
    return PrepareResult::UNRECOGNIZED_STATEMENT;
}

PrepareResult DB::prepare_insert(std::string s, Statement &statement) {
    using std::string;
    std::vector<string> out;
    while (!s.empty()) {
        while (s.at({}) == ' ' && !s.empty()) {
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

    const string id_string = out.at(1);
    const string username = out.at(2);
    const string email = out.at(3);

    int id;
    try {
        id = std::stoi(id_string);
    }
    catch (std::invalid_argument) {
        return PrepareResult::SYNTAX_ERROR;
    }

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

bool DB::parse_statement(const std::string &input_line, Statement &statement) {
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

MetaCommandResult DB::do_meta_command(const std::string &command) {
    if (command == ".exit") {
        table.reset(nullptr);
        std::cout << "Bye!" << std::endl;
        exit(EXIT_SUCCESS);
    }
    if (command == ".help") {
        return MetaCommandResult::HELP;
    }
    return MetaCommandResult::UNRECOGNIZED_COMMAND;
}

bool DB::parse_meta_command(const std::string &command) {
    if (command[0] == '.') {
        switch (do_meta_command(command)) {
            case MetaCommandResult::HELP:
                std::cout << ".help : 打印帮助" << std::endl;
                std::cout << ".exit : 退出" << std::endl;
                return true;
            case MetaCommandResult::UNRECOGNIZED_COMMAND:
                std::cout << "Unrecognized command:" << command << std::endl;
                return true;
            case MetaCommandResult::SUCCESS:
                return true;
        }
    }
    return false;
}

ExecuteResult DB::execute_insert(const Statement &statement) {
    if (table->num_rows >= TABLE_MAX_ROWS) {
        return ExecuteResult::TABLE_FULL;
    }
    void *page = table->row_slot(table->num_rows);
    
    serialize_row(statement.row_to_insert, page);
    table->num_rows++;

    return ExecuteResult::SUCCESS;
}

ExecuteResult DB::execute_select() {
    for (uint32_t i = 0; i < table->num_rows; i++) {
        Row row;
        void *page = table->row_slot(i);
        deserialize_row(page, row);
        std::cout << "(" << row.id << ", " << row.username << ", " << row.email << ")" << std::endl;
    }
    return ExecuteResult::SUCCESS;
}

void DB::execute_statement(const Statement &statement) {
    ExecuteResult executeResult;
    switch (statement.type) {
        case StatementType::INSERT:
            executeResult = execute_insert(statement);
            break;
        case StatementType::SELECT:
            executeResult = execute_select();
            break;
        default:
            executeResult = ExecuteResult::ERROR;
    }

    switch (executeResult) {
        case ExecuteResult::SUCCESS:
            std::cout << "Executed." << std::endl;
            break;
        case ExecuteResult::TABLE_FULL:
            std::cout << "Error : Table full." << std::endl;
            break;
        default:
            std::cout << "Error : Uncategorised error;";
    }
}
