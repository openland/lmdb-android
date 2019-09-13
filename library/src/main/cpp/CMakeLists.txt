cmake_minimum_required(VERSION 3.4.1)

add_library(
        lmdbjni SHARED

        lmdbjni.cpp
        lmdb/mdb.c
        lmdb/midl.c
)
find_library(log-lib log)

target_link_libraries(lmdbjni ${log-lib})