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

set(SOURCES_READER reader.cpp message_helpers.cpp ../include/message_helpers.h)
# source file for reader
add_executable(reader ${SOURCES_READER})
# include directory
target_include_directories(reader PRIVATE ../include)
## add the include directory to our compile directives
target_include_directories(reader PUBLIC ${ZeroMQ_INCLUDE_DIR})
## at the 0mq library to our link directive
target_link_libraries(reader PUBLIC ${ZeroMQ_LIBRARY})
