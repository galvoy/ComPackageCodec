cmake_minimum_required(VERSION 3.29)
project(c C)

set(CMAKE_C_STANDARD 11)

add_executable(c main.c
        ComPackageCRC.c
        ComPackageCRC.h
        ComPackageCodec.c
        ComPackageCodec.h
)
