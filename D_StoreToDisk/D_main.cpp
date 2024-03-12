// Copyright (c) DaTouJun 2022 11
// SetForGroup MCOTF

//
// Created by DaTouJun on 2022/11/4.
//

#include <iostream>
#include "D_DB.hpp"

#define EXIT_SUCCESS 0


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Must supply a database filename." << std::endl;
        exit(EXIT_FAILURE);
    }
    DB db(argv[1]);
    db.start();
    return 0;
}