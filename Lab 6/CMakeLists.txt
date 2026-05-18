cmake_minimum_required(VERSION 4.1)
project(Lab_5)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lab_5 main.c
        circular_queue.c
        simple_queue.c
        deque.c
        stack.c
        priority_queue.c
        unionBank.c)
