cmake_minimum_required(VERSION 3.5)

project(reciver LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

include_directories("/usr/include/SFML/")
link_directories("/usr/lib/x86_64-linux-gnu/")

add_executable(reciver main.cpp buffermsg.cpp buffermsg.h signalrecoder.cpp signalrecoder.h)
target_link_libraries(reciver sfml-audio sfml-system)
