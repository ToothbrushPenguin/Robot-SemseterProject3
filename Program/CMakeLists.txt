cmake_minimum_required(VERSION 3.5)

project(Program LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("/usr/include/SFML/")
link_directories("/usr/lib/x86_64-linux-gnu/")


add_executable(Program main.cpp
    msghandeler.cpp msghandeler.h
    msgbuffer.cpp msgbuffer.h
    buffermsg.cpp buffermsg.h signalrecoder.cpp signalrecoder.h

    )
target_link_libraries(Program sfml-audio)
