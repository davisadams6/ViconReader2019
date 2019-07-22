# hello world example
add_executable(test cmakeTest.cpp)

## load in pkg-config support
find_package(PkgConfig)
## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_ZeroMQ QUIET zmq)

## use the hint from above to find where 'zmq.hpp' is located
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.h
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

## use the hint from about to find the location of libzmq
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

# source file for hwserver server
add_executable(hwserver hwserver.c)
# source file for hwclient server
add_executable(hwclient hwclient.c)

## add the include directory to our compile directives
target_include_directories(hwserver PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_include_directories(hwclient PUBLIC ${ZeroMQ_INCLUDE_DIR})
## at the 0mq library to our link directive
target_link_libraries(hwserver PUBLIC ${ZeroMQ_LIBRARY})
target_link_libraries(hwclient PUBLIC ${ZeroMQ_LIBRARY})
