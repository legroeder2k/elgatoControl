set(DAEMON_SOURCES
        main.cpp AvahiBrowser.cpp Log.cpp ElgatoLight.cpp HTTPRequest.hpp ElgatoServerImpl.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(PkgConfig REQUIRED)
find_package(Avahi REQUIRED)
find_package(nlohmann_json 3.10.5 REQUIRED)
find_package(fmt REQUIRED)

pkg_search_module(UUID REQUIRED uuid)

add_compile_options(-Wall -Wextra -pedantic -Werror)

message(STATUS "Build type for daemon: ${CMAKE_BUILD_TYPE}")

add_executable(elgatoDaemon ${DAEMON_SOURCES})
target_link_libraries(elgatoDaemon PUBLIC Avahi::client nlohmann_json::nlohmann_json fmt::fmt ${UUID_LIBRARIES})
target_link_libraries(elgatoDaemon PRIVATE elgatoProto)