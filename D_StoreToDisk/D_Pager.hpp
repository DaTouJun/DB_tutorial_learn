//
// Created by 28318 on 24-3-10.
//

#ifndef PAGER_HPP
#define PAGER_HPP

#include "D_Enums.hpp"

#include <cstdio>
#include <fcntl.h>

class Pager {
public:
    int file_describer;
    uint32_t file_length;
    void *pages[TABLE_MAX_PAGES];

    explicit Pager(const char *filename) : pages(nullptr) {
        file_describer = open(filename,
                              O_RDWR |
                              O_CREAT,
                              S_IWUSR |
                              S_IRUSR);

        if (file_describer < 0) {
            std::cerr << "Error : cannot open file " << filename << std::endl;
            exit(EXIT_FAILURE);
        }
        file_length = lseek(file_describer, 0, SEEK_END);

        for (auto &page: pages) {
            page = nullptr;
        }
    }

    void *get_page(uint32_t page_num) {
        if (page_num >= TABLE_MAX_PAGES) {
            std::cout << "Tried to fetch page number out of bounds. " << page_num
                      << " > " << TABLE_MAX_PAGES << std::endl;
            exit(EXIT_FAILURE);
        }
        if (pages[page_num] == nullptr) {
            // Cache miss. Allocate memory and load from file.
            void *page = malloc(TableSettings::PAGE_SIZE);
            uint32_t num_pages = file_length / TableSettings::PAGE_SIZE;

            if (file_length % TableSettings::PAGE_SIZE) {
                num_pages++;
            }

            // We might save a partial page at the end of the file
            if (file_length <= num_pages) {
                lseek(file_describer, page_num * TableSettings::PAGE_SIZE,
                      SEEK_SET);
                ssize_t bytes_read = read(file_describer,
                                          page, TableSettings::PAGE_SIZE);
                if (bytes_read == -1) {
                    std::cout << "Error reading file" << errno << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            pages[page_num] = page;
        }
        return pages[page_num];
    }

    void pager_flush(uint32_t page_num, uint32_t size) {
        if (pages[page_num] == nullptr) {
            std::cout << "Tried to flush null page " << std::endl;
            exit(EXIT_FAILURE);
        }
        off_t offset = lseek(file_describer,
                             TableSettings::PAGE_SIZE,
                             SEEK_SET);
        if (offset == -1) {
            std::cout << "Error seeking: " << errno << std::endl;
            exit(EXIT_FAILURE);
        }

        ssize_t bytes_written =
                write(file_describer, pages[page_num], size);
        if (bytes_written == -1) {
            std::cout << "Error writing : " << errno << std::endl;
            exit(EXIT_FAILURE);
        }
    }
};

#endif //PAGER_HPP
