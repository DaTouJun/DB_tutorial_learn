//
// Created by 28318 on 24-3-10.
//

#ifndef PAGER_HPP
#define PAGER_HPP

#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

class Pager {
public:
    int file_describer;
    uint32_t file_length;
    void *pages[TABLE_MAX_PAGES];
    uint32_t num_pages;

    // 打开文件
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
        num_pages = file_length / PAGE_SIZE;
        if (file_length % PAGE_SIZE != 0) {
            std::cerr << "Db file is not a whole number of pages. Corrupt file." << std::endl;
            exit(EXIT_FAILURE);
        }

        for (auto &page: pages) {
            page = nullptr;
        }
    }

    // 通过编号获得页面
    void *get_page(uint32_t page_num) {
        if (page_num >= TABLE_MAX_PAGES) {  // 超过了最大页面数
            std::cout << "Tried to fetch page number out of bounds. " << page_num
                      << " > " << TABLE_MAX_PAGES << std::endl;
            exit(EXIT_FAILURE);
        }
        if (pages[page_num] == nullptr) {   // 查看试图访问的页面号是否存在，否则从磁盘加载
            // Cache miss. Allocate memory and load from file.
            void *page = malloc(PAGE_SIZE);
            uint32_t num_pages = file_length / PAGE_SIZE;   // 已经有的页面数

            if (file_length % PAGE_SIZE) {
                num_pages++;
            }

            // We might save a partial page at the end of the file
            if (page_num <= num_pages) {    // 获得的页面已经有了 则进行读取
                lseek(file_describer, page_num * PAGE_SIZE,
                      SEEK_SET);
                ssize_t bytes_read = read(file_describer,
                                          page, PAGE_SIZE);
                if (bytes_read == -1) {
                    std::cout << "Error reading file" << errno << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            pages[page_num] = page;
            if (page_num >= num_pages) {
                this->num_pages = page_num + 1;
            }
        }
        return pages[page_num];
    }

    void pager_flush(uint32_t page_num) {
        if (pages[page_num] == nullptr) {
            std::cout << "Tried to flush null page " << std::endl;
            exit(EXIT_FAILURE);
        }
        off_t offset = lseek(file_describer,
                             PAGE_SIZE * page_num,
                             SEEK_SET);
        if (offset == -1) {
            std::cout << "Error seeking: " << errno << std::endl;
            exit(EXIT_FAILURE);
        }

        ssize_t bytes_written =
                write(file_describer, pages[page_num], PAGE_SIZE);
        if (bytes_written == -1) {
            std::cout << "Error writing : " << errno << std::endl;
            exit(EXIT_FAILURE);
        }
    }
};

#endif //PAGER_HPP
