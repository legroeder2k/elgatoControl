cmake_minimum_required(VERSION 3.22)
project(elgatoControl VERSION 0.1 DESCRIPTION "Controlling elgato fixtures using a client/server approach")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CmakeScripts)

if (NOT DEFINED CONFIG_PATH)
set(CONFIG_PATH ~/.config/elgatoControl)
endif()

if (NOT DEFINED SOCKET_FILE)
    set(SOCKET_FILE ~/.local/run/elgatoDaemon)
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

configure_file(${CMAKE_SOURCE_DIR}/elgatoDaemon/Config.h.in ${CMAKE_SOURCE_DIR}/elgatoDaemon/Config.h)

add_subdirectory(elgatoDaemon)
add_subdirectory(proto)