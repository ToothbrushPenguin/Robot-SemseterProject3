cmake_minimum_required(VERSION 3.5)

project(Computer LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("/usr/include/SFML/")
link_directories("/usr/lib/x86_64-linux-gnu/")

add_executable(Computer main.cpp msgbuffer.cpp msgbuffer.h seqhandler.cpp seqhandler.h msghandeler.cpp msghandeler.h)
target_link_libraries(Computer sfml-audio)

