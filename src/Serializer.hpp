//
// Created by 28318 on 2023/3/9.
//

#ifndef D_SERIALIZER
#define D_SERIALIZER

#include "Row.hpp"
#include "EnumsAndDefs.hpp"
#include <cstring>

void static serialize_row(const Row &source, void *destination) {
    memcpy(static_cast<char *>(destination) + ID_OFFSET, &source.id, ID_SIZE);
    memcpy(static_cast<char *>(destination) + USERNAME_OFFSET, &source.username, USERNAME_SIZE);
    memcpy(static_cast<char *>(destination) + EMAIL_OFFSET, &source.email, EMAIL_SIZE);
}

void static deserialize_row(void *source, Row &destination) {
    memcpy(&destination.id, static_cast<char *>(source) + ID_OFFSET, ID_SIZE);
    memcpy(&destination.username, static_cast<char *>(source) + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&destination.email, static_cast<char *>(source) + EMAIL_OFFSET, EMAIL_SIZE);
}



#endif //D_SERIALIZER
