cmake_minimum_required(VERSION 3.9)
project(am_svr)

include_directories(./common/include)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "-O0")

add_executable(am_svr main.cpp am_malloc.h am_malloc.cpp am_memory.h session.cpp session.h buffer.h session_mng.h session_mng.cpp)
target_link_libraries(am_svr boost_system boost_thread boost_serialization)
