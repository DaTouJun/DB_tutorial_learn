//
// Created by 28318 on 2023/3/9.
//

#ifndef C_ROW_H
#define C_ROW_H

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

class Row {
public:
    unsigned int id{0};
    char username[COLUMN_USERNAME_SIZE + 1]{'\0'};
    char email[COLUMN_EMAIL_SIZE + 1]{'\0'};
};

#endif //C_ROW_H
