set(CLI_SOURCES
        main.cpp ElgatoClient.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(fmt REQUIRED)

add_compile_options(-Wall -Wextra -pedantic -Werror)

message(STATUS "Build type for cli: ${CMAKE_BUILD_TYPE}")

add_executable(elgato-cli ${CLI_SOURCES})
target_link_libraries(elgato-cli PUBLIC fmt::fmt)
target_link_libraries(elgato-cli PRIVATE elgatoProto)

