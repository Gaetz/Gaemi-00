cmake_minimum_required(VERSION 3.20)
project(gassets)
set(CMAKE_CXX_STANDARD 20)

file( GLOB GASSETS_SOURCES *.cpp )
add_library( gassets ${GASSETS_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gassets raylib gfile glog)