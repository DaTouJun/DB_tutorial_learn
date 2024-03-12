//
// Created by 28318 on 2023/3/9.
//

#ifndef D_ROW
#define D_ROW

#include <string>
#include <cstring>


constexpr int COLUMN_USERNAME_SIZE = 32;
constexpr int COLUMN_EMAIL_SIZE = 255;

// 最大页数量
constexpr int TABLE_MAX_PAGES = 100;

class Row {
public:
    unsigned int id{0};
    char username[COLUMN_USERNAME_SIZE + 1]{'\0'};
    char email[COLUMN_EMAIL_SIZE + 1]{'\0'};

    Row(const unsigned int id, const std::string&username, const std::string&email) {
        this->id = id;
        strncpy(this->username, username.c_str(), COLUMN_USERNAME_SIZE);
        this->username[COLUMN_USERNAME_SIZE] = '\0';
        strncpy(this->email, email.c_str(), COLUMN_EMAIL_SIZE);
        this->email[COLUMN_EMAIL_SIZE] = '\0';
    }

    Row() = default;
};

#endif //D_ROW
