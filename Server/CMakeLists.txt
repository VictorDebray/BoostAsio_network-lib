cmake_minimum_required(VERSION 3.13)
project(Server)

set(CMAKE_CXX_STANDARD 14)

set(NETWORK_DIR ../Network)
set(COMMON_DIR ../Common)

include_directories(${COMMON_DIR})
include_directories(${NETWORK_DIR})

set(NETWORK_SRC ${NETWORK_DIR}/Accept.cpp ${NETWORK_DIR}/Connection.cpp)
set(COMMON_SRC ${COMMON_DIR}/request.pb.cc)
set(SOURCES Server.cpp Server.hpp ServerNetwork.cpp ServerNetwork.hpp ClientConnection.cpp ClientConnection.hpp)

include(FindProtobuf)
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})

add_executable(server main.cpp ${SOURCES} ${NETWORK_SRC} ${COMMON_SRC})

if (APPLE OR WIN32)
    set(Boost_USE_STATIC_LIBS TRUE)
endif(APPLE OR WIN32)
set(Boost_DEBUG off)
find_package(Boost 1.65.1 COMPONENTS system thread serialization regex REQUIRED)
include_directories( ${Boost_INCLUDE_DIR} )


if(APPLE OR UNIX)
    target_link_libraries(server ${Boost_LIBRARIES} -lpthread ${PROTOBUF_LIBRARY})
elseif(WIN32)
    target_link_libraries(server ${Boost_LIBRARIES})
endif()

install(TARGETS server DESTINATION ${PROJECT_SOURCE_DIR}/../bin)
