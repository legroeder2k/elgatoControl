cmake_minimum_required(VERSION 3.22)
project(elgatoControl)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CmakeScripts)

add_subdirectory(elgatoDaemon)