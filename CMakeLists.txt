cmake_minimum_required(VERSION 2.8)
project(EpollPthreadPoll)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        condvar.cpp
        condvar.h
        epoll.cpp
        epoll.h
        errexit.h
        main.cpp
        mutex.cpp
        mutex.h
        pthreadpoll.cpp
        pthreadpoll.h
        task.cpp
        task.h
        tcpsocket.cpp
        tcpsocket.h)

add_executable(EpollPthreadPoll ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread)