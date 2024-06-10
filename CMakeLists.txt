cmake_minimum_required(VERSION 3.15)

project(Utils VERSION 1.0.0 LANGUAGES CXX)
 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CACHE_TEST_NAME cache_test)
add_executable(${CACHE_TEST_NAME} app/CacheTest.cpp)
configure_file(UtilsConfig.h.in UtilsConfig.h)

find_package(GTest)
find_package(GFlags)

# target_include_directories(
#     ${CAHCE_TEST_NAME} PUBLIC
#     ${PROJECT_BINARY_DIR}
#     ${GTEST_INCLUDE_DIRS}
#     ${GFLAGS_INCLUDE_DIRS}
# )

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(cache)

MESSAGE(${GTEST_BOTH_LIBRARIES})
MESSAGE(${GTEST_LIBRARIES})
MESSAGE(${GTEST_MAIN_LIBRARIES})

target_link_libraries(
    ${CACHE_TEST_NAME} gflags
)

# target_link_libraries(
#     ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES}
# )
string(TIMESTAMP COMPILE_TIME %Y%m%d-%H%M%S)
