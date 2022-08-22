cmake_minimum_required(VERSION 3.22)
project(elgatoControl VERSION 0.1 DESCRIPTION "Controlling elgato fixtures using a client/server approach")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CmakeScripts)

if (NOT DEFINED CONFIG_PATH)
set(CONFIG_PATH "$[HOME]/.config/elgatoControl")
endif()

if (NOT DEFINED SOCKET_FILE)
    set(SOCKET_FILE "$[HOME]/.local/elgatoDaemon.sock")
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ~/.local CACHE PATH "default install path" FORCE)
endif()

configure_file(${CMAKE_SOURCE_DIR}/elgatoDaemon/Config.h.in ${CMAKE_SOURCE_DIR}/Config.h)

add_subdirectory(elgatoDaemon)
add_subdirectory(elgato-cli)
add_subdirectory(proto)

message(STATUS "Install to ${CMAKE_INSTALL_PREFIX}")

file(READ additional/elgatoDaemon.service.template FILE_CONTENTS)
string(REPLACE "[INSTALL_DIR]" "${CMAKE_INSTALL_PREFIX}" FILE_CONTENTS "${FILE_CONTENTS}")
file(WRITE additional/elgatoDaemon.service "${FILE_CONTENTS}")

INSTALL(TARGETS elgatoDaemon RUNTIME)
INSTALL(TARGETS elgato-cli RUNTIME)
INSTALL(FILES additional/elgatoDaemon.service DESTINATION ~/.config/systemd/user)