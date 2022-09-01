set (UI_SOURCES
        main.cpp Tray.cpp ElgatoClient.cpp FixtureMenuItem.cpp SettingsWindow.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
add_compile_options(-Wall -Wextra -pedantic -Werror)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0)

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

message(STATUS "Build type for ui: ${CMAKE_BUILD_TYPE}")

add_executable(elgato-ui ${UI_SOURCES})
target_link_libraries(elgato-ui PUBLIC ${GTKMM_LIBRARIES})
target_link_libraries(elgato-ui PRIVATE elgatoProto)