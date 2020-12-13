cmake_minimum_required(VERSION 3.16.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options ("/DIMGUI_IMPL_OPENGL_LOADER_GLAD;/DP_DEBUG;")

project("Plutus" C CXX)


set(LIBS_DIR ${PROJECT_SOURCE_DIR}/libs)
# Add source files
file(GLOB_RECURSE PLUTUS_SRC 
	${PROJECT_SOURCE_DIR}/src/*.c
	${PROJECT_SOURCE_DIR}/src/*.cpp
	${PROJECT_SOURCE_DIR}/libs/glad/src/glad.c
	${LIBS_DIR}/stb/stb_vorbis.c
	${LIBS_DIR}/stb/stb_image.c)
	
# Add header files
file(GLOB_RECURSE PLUTUS_INC
	${PROJECT_SOURCE_DIR}/src/*.h
	${PROJECT_SOURCE_DIR}/src/*.hpp
	${PROJECT_SOURCE_DIR}/libs/glad/include/*.h
	${LIBS_DIR}/stb/stb_vorbis.h
	${LIBS_DIR}/stb/stb_image.h)


include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${LIBS_DIR})
include_directories(${LIBS_DIR}/rapidjson/include)
include_directories(${LIBS_DIR}/freetype/include)
include_directories(${LIBS_DIR}/glad/include)
include_directories(${LIBS_DIR}/SDL/include)
include_directories(${LIBS_DIR}/glm)
include_directories(${LIBS_DIR}/stb)

add_library(Plutus STATIC ${PLUTUS_SRC} ${PLUTUS_INC})