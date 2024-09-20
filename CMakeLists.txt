cmake_minimum_required(VERSION 3.5)

project(bayan LANGUAGES CXX)

find_package(Boost COMPONENTS program_options filesystem regex REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(bayan main.cpp dirtraveler.h)

target_link_libraries(bayan
        ${Boost_LIBRARIES}
        )
