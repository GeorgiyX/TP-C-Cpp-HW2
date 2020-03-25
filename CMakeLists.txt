PROJECT(TP_C_Cpp_HW2 C)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10.2)
SET(CMAKE_C_STANDARD 11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
#TODO: CODE COV.

INCLUDE_DIRECTORIES(project/include)

IF(IS_SINGLE_THREAD OR (NOT IS_SINGLE_THREAD AND NOT IS_SINGLE_MULTI_THREAD))
    ADD_LIBRARY(sequence_search_static STATIC project/src/seq_search_st.c project/include/search.h project/src/search.c)
ENDIF()
IF(IS_SINGLE_MULTI_THREAD OR (NOT IS_SINGLE_THREAD AND NOT IS_SINGLE_MULTI_THREAD))
    ADD_LIBRARY(sequence_search_dynamic SHARED project/src/seq_search_mt.c project/include/search.h project/src/search.c project/include/search_mt.h project/src/search_mt.c)
    TARGET_LINK_LIBRARIES(sequence_search_dynamic pthread)
ENDIF()

ADD_SUBDIRECTORY(project/testing)