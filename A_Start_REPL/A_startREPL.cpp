#include <iostream>
#include <string>

/*
 * 什么叫REPL
 * 读取-求值-输出 循环
 * Read-Eval-Print Loop
 * 交互式顶层构件
 * interactive toplevel
 */

namespace DB {
    inline void printPrompt() {
        std::cout << "db-> ";
    }

    bool parse_meta_command(const std::string &command) {
        if (command == ".exit") {
            std::cout << "Bye!" << std::flush;
            return false;
        } else {
            std::cout << "Unrecognized command: " << command << std::endl;
            return true;
        }
    }

    [[noreturn]] void start() {
        while (true) {
            DB::printPrompt();

            std::string input_line;
            std::getline(std::cin, input_line);

            if (!parse_meta_command(input_line)) {
                break;
            }
        }
        exit(0);
    }
} // DB_start


int main() {
    DB::start();
}