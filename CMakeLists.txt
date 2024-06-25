cmake_minimum_required(VERSION 3.28)
project(asgn3 C)

set(CMAKE_C_STANDARD 11)

add_executable(asgn3 main.c
        queue.c
        queue.h
        rwlock.c
        rwlock.h)
