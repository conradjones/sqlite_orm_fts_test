cmake_minimum_required(VERSION 3.30)
project(sqtest)

set(CMAKE_CXX_STANDARD 20)

find_package(sqlite3 REQUIRED)
find_package(SqliteOrm REQUIRED CONFIG)

add_executable(sqtest main.cpp)


target_link_libraries(sqtest  SQLite::SQLite3)
target_link_libraries(sqtest  sqlite_orm::sqlite_orm)