cmake_minimum_required(VERSION 3.20)
project(gdraw)
set(CMAKE_CXX_STANDARD 20)

file( GLOB GDRAW_SOURCES *.cpp )
add_library( gdraw ${GDRAW_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gdraw raylib gassets gplatform gmath glog)