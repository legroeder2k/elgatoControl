set(DAEMON_SOURCES
        main.cpp AvahiBrowser.cpp AvahiBrowser.h Log.cpp Log.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Avahi REQUIRED)
find_package(Threads REQUIRED)

add_executable(elgatoDaemon ${DAEMON_SOURCES})
target_link_libraries(elgatoDaemon Avahi::client Threads::Threads)