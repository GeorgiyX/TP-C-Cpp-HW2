PROJECT(stress_and_time_test)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10.2)
SET(CMAKE_C_STANDARD 11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

INCLUDE_DIRECTORIES(include)

ADD_EXECUTABLE(stress_test_single src/main.c include/utils.h src/utils.c) #!
TARGET_LINK_LIBRARIES(stress_test_single sequence_search_static)

ADD_EXECUTABLE(stress_test_multi src/main.c include/utils.h src/utils.c) #!
TARGET_COMPILE_DEFINITIONS(stress_test_multi PUBLIC -D_DYNAMIC)
TARGET_LINK_LIBRARIES(stress_test_multi dl)

