cmake_minimum_required(VERSION 3.5)

project(Robot LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories("/usr/include/SFML/")
link_directories("/usr/lib/x86_64-linux-gnu/")


find_package(PahoMqttCpp REQUIRED)

include_directories("inc")


add_executable(
    Robot
    main.cpp
    ../Classes/msghandler.cpp      ../Classes/msghandler.h
    ../Classes/msgbuffer.cpp       ../Classes/msgbuffer.h
    ../Classes/buffermsg.cpp       ../Classes/buffermsg.h
    ../Classes/robotmovement.cpp   ../Classes/robotmovement.h
    ../Classes/signalrecorder.cpp  ../Classes/signalrecorder.h
    )
target_link_libraries(Robot PahoMqttCpp::paho-mqttpp3 sfml-audio sfml-system )
