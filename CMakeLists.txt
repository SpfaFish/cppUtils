cmake_minimum_required(VERSION 3.15)

project(Utils VERSION 1.0.0 LANGUAGES CXX)
 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Utils tests/CacheTest.cpp)
configure_file(UtilsConfig.h.in UtilsConfig.h)

find_package(GTest)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${PROJECT_BINARY_DIR}
    ${GTEST_INCLUDE_DIRS}
)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(cache)

MESSAGE(${GTEST_BOTH_LIBRARIES})
MESSAGE(${GTEST_LIBRARIES})
MESSAGE(${GTEST_MAIN_LIBRARIES})

target_link_libraries(
    ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES}
)
target_link_libraries(
    ${PROJECT_NAME} Cache
)
string(TIMESTAMP COMPILE_TIME %Y%m%d-%H%M%S)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)