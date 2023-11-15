//
// Created by 28318 on 2023/3/9.
//

#ifndef C_SERIALIZE_H
#define C_SERIALIZE_H

#include "C_Row.h"
#include <memory>
#include <cstring>


#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)
constexpr int ID_SIZE = size_of_attribute(Row, id);
constexpr int USERNAME_SIZE = size_of_attribute(Row, email);
constexpr int EMAIL_SIZE = size_of_attribute(Row, email);

constexpr int ID_OFFSET = 0;
constexpr int USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
constexpr int EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;

constexpr int ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

void static serialize_row(const Row &source, void *destination) {
    memcpy(static_cast<char *>(destination) + ID_OFFSET, &(source.id), ID_SIZE);
    memcpy(static_cast<char *>(destination) + USERNAME_OFFSET, &(source.username), USERNAME_SIZE);
    memcpy(static_cast<char *>(destination) + EMAIL_OFFSET, &(source.email), EMAIL_SIZE);
}

void static deserialize_row(void *source, Row &destination) {
    memcpy(&(destination.id), static_cast<char *>(source) + ID_OFFSET, ID_SIZE);
    memcpy(&(destination.username), static_cast<char *>(source) + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination.email), static_cast<char *>(source) + EMAIL_OFFSET, EMAIL_SIZE);
}


#endif //C_SERIALIZE_H
