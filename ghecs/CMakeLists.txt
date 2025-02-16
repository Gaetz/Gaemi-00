cmake_minimum_required(VERSION 3.20)
project(ghecs)
set(CMAKE_CXX_STANDARD 20)

file( GLOB GHECS_SOURCES *.cpp )
add_library( ghecs ${GHECS_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ghecs gplatform gmath glog)