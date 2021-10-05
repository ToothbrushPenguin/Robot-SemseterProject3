cmake_minimum_required(VERSION 3.5)

project(robotMovement LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(robotMovement main.cpp robotmovement.cpp robotmovement.h)

# Find the PahoMqttCpp package
find_package(PahoMqttCpp REQUIRED)

include_directories("inc")

target_link_libraries(robotMovement PahoMqttCpp::paho-mqttpp3)
