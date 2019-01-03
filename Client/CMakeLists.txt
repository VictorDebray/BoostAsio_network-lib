cmake_minimum_required(VERSION 3.13)
project(Client)

set(CMAKE_CXX_STANDARD 14)

set(COMMON_DIR ../Common)
set(NETWORK_DIR ../Network)

include_directories(${COMMON_DIR})
include_directories(${NETWORK_DIR})

set(COMMON_SRC ${COMMON_DIR}/request.pb.cc ${COMMON_DIR}/PacketFactory.cpp)
set(NETWORK_SRC ${NETWORK_DIR}/Connection.cpp)
set(SOURCES Client.cpp ServerConnection.cpp)

include(FindProtobuf)
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})

add_executable(client main.cpp ${SOURCES} ${COMMON_SRC} ${NETWORK_SRC})

if (APPLE OR WIN32)
    set(Boost_USE_STATIC_LIBS TRUE)
endif(APPLE OR WIN32)
set(Boost_DEBUG off)
find_package(Boost 1.66 COMPONENTS system thread serialization regex REQUIRED)
include_directories( ${Boost_INCLUDE_DIR} )


if(APPLE OR UNIX)
    target_link_libraries(client ${Boost_LIBRARIES} -lpthread ${PROTOBUF_LIBRARY})
elseif(WIN32)
    target_link_libraries(client ${Boost_LIBRARIES})
endif()

install(TARGETS client DESTINATION ${PROJECT_SOURCE_DIR}/../bin)
