CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(Particle2D)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) 
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

if (MSVC) 
	ADD_DEFINITIONS("/W4") # -W5 is too strict
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")
else()
	ADD_DEFINITIONS("-Wall -std=c++11")
endif()

MACRO(ADD_FILTER DIR_NAME ALL_SRC)
    FILE(GLOB_RECURSE FILTER_SRC FOLLOW_SYMLINKS ${DIR_NAME}/*.cpp ${DIR_NAME}/*.h)
    SOURCE_GROUP(${DIR_NAME} FILES ${FILTER_SRC})
    SET(${ALL_SRC} ${${ALL_SRC}} ${FILTER_SRC})
ENDMACRO(ADD_FILTER)

ADD_SUBDIRECTORY(src Particle2D)
ADD_SUBDIRECTORY(test Test)
ADD_SUBDIRECTORY(ios IOSRender)
