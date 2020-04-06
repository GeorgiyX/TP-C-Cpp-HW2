PROJECT(.)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10.2)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g ")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

FIND_PACKAGE(GTest REQUIRED)
INCLUDE_DIRECTORIES("${GTEST_INCLUDE_DIRS}")

FIND_PACKAGE(Threads REQUIRED)
INCLUDE_DIRECTORIES(include ../../include)
ADD_EXECUTABLE(testing src/main.cpp src/st_tests.cpp src/utils.cpp src/mt_tests.cpp)

TARGET_LINK_LIBRARIES(testing ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} sequence_search_static dl stdc++fs)

ADD_TEST(
        NAME MAIN_TEST
        COMMAND testing ./project/testing/data/ ./build-testing/libsequence_search_dynamic.so WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
