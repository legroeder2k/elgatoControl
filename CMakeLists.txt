cmake_minimum_required(VERSION 3.22)
project(elgatoControl)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CmakeScripts)

set(CONFIG_PATH ~/.config/elgatoControl)
set(SOCKET_FILE ~/.local/run/elgatoDaemon)

configure_file(${CMAKE_SOURCE_DIR}/elgatoDaemon/Config.h.in ${CMAKE_SOURCE_DIR}/elgatoDaemon/Config.h)

add_subdirectory(elgatoDaemon)