cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 23)

project(A_start_demo)
add_executable(A_start_demo
        A_Start_REPL/A_startREPL.cpp)

add_custom_command(TARGET A_start_demo
        POST_BUILD
        COMMAND rspec ${CMAKE_SOURCE_DIR}/A_Start_REPL/A_Test.rb
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
#set_property(TARGET A_start_demo PROPERTY OUTPUT_NAME db)

project(B_FrontEnd_demo)
add_executable(B_FrontEnd_demo
        B_FrontEnd/B_FrontEnd.cpp)
add_custom_command(TARGET B_FrontEnd_demo
        POST_BUILD
        COMMAND rspec ${CMAKE_SOURCE_DIR}/B_FrontEnd/B_DB_TestOfFrontEnd.rb
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

project(C_insertAndSelect)
add_executable(C_insertAndSelect
        C_InsertAndSelect/C_serialize.hpp
        C_InsertAndSelect/C_insertAndSelect.cpp
        C_InsertAndSelect/C_Row.h
        C_InsertAndSelect/C_classTable.h
        C_InsertAndSelect/C_DB.cpp
        C_InsertAndSelect/C_DB.h
        C_InsertAndSelect/C_Enums.h)
add_custom_command(TARGET C_insertAndSelect
        POST_BUILD
        COMMAND rspec ${CMAKE_SOURCE_DIR}/C_insertAndSelect/C_TestInsertAndSelect.rb
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

project(D_StoreToDisk)
project(D_StoreToDiskWithoutTest)

file(GLOB_RECURSE ProjectD_SrcFiles "D_StoreToDisk/*.cpp" "D_StoreToDisk/*.hpp")

add_executable(D_StoreToDiskWithoutTest ${ProjectD_SrcFiles})

add_executable(D_StoreToDisk ${ProjectD_SrcFiles})
add_custom_command(TARGET D_StoreToDisk
        POST_BUILD
        COMMAND rspec ${CMAKE_SOURCE_DIR}/Tests/D_StoreToDiskTest.rb
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)



