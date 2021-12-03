cmake_minimum_required(VERSION 3.5)

project(Computer LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("/usr/include/SFML/")
link_directories("/usr/lib/x86_64-linux-gnu/")

add_executable(
    Computer
    main.cpp
    ../Classes/seqhandler.cpp       ../Classes/seqhandler.h
    ../Classes/ui.cpp 		    ../Classes/ui.h
    ../Classes/msghandler.cpp      ../Classes/msghandler.h
    ../Classes/msgbuffer.cpp       ../Classes/msgbuffer.h
    ../Classes/buffermsg.cpp       ../Classes/buffermsg.h
    ../Classes/signalrecorder.cpp  ../Classes/signalrecorder.h
)

target_link_libraries(Computer sfml-audio sfml-system)

