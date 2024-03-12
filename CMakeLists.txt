cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 23)

# 第四个 存储到磁盘
project(tinyDB)
project(tinyDB_WithoutTest)

file(GLOB_RECURSE ProjectD_SrcFiles "src/*.cpp" "src/*.hpp")

add_executable(tinyDB_WithoutTest ${ProjectD_SrcFiles})

add_executable(tinyDB ${ProjectD_SrcFiles})
add_custom_command(TARGET tinyDB
        POST_BUILD
        COMMAND rm test.db
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
add_custom_command(TARGET tinyDB
        POST_BUILD
        COMMAND rspec ${CMAKE_SOURCE_DIR}/test/tinyDB.rb
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)



