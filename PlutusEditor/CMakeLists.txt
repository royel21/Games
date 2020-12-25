cmake_minimum_required(VERSION 3.16.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options ("/DP_DEBUG;")

project("PlutusEditor" C CXX)


set(LIBS_DIR ${CMAKE_SOURCE_DIR}/Plutus/libs)
# Add source files
file(GLOB_RECURSE EDITOR_SRC 
	${PROJECT_SOURCE_DIR}/*.c
	${PROJECT_SOURCE_DIR}/*.cpp)
	
# Add header files
file(GLOB_RECURSE EDITOR_INC
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.hpp)


include_directories(${CMAKE_SOURCE_DIR}/Plutus/src)
include_directories(${PROJECT_SOURCE_DIR}/)
include_directories(${LIBS_DIR}/)
include_directories(${LIBS_DIR}/glad/include)
include_directories(${LIBS_DIR}/SDL/include)
include_directories(${LIBS_DIR}/glm)
include_directories(${LIB_DIR}/Entt)
include_directories(${LIB_DIR}/spdlog/include)

add_library(PlutusEditor STATIC ${EDITOR_SRC} ${EDITOR_INC})