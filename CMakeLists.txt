cmake_minimum_required(VERSION 3.16.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("cmake_test" CXX)

add_compile_options ("/DIMGUI_IMPL_OPENGL_LOADER_GLAD;/DP_DEBUG;")
	
set(LIB_DIR  ${CMAKE_SOURCE_DIR}/Plutus/libs)
message(${LIB_DIR}/imgui/*.cpp)
# Add source files
file(GLOB_RECURSE SRC_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp
	${LIB_DIR}/imgui/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp,
	${LIB_DIR}/imgui/*.h)

include_directories(${CMAKE_SOURCE_DIR}/Plutus/src)
include_directories(${LIB_DIR}/glad/include)
include_directories(${LIB_DIR}/glm)
include_directories(${LIB_DIR}/lua/include)
include_directories(${LIB_DIR}/rapidjson/include)
include_directories(${LIB_DIR}/SDL/include)
include_directories(${LIB_DIR}/freetype/include)
include_directories(${LIB_DIR}/imgui)
include_directories(${LIB_DIR}/Entt)

set(SDL_DIR ${LIB_DIR}/SDL/lib/x86)

set(SDL ${SDL_DIR}/SDL2main.lib ${SDL_DIR}/SDL2.lib)

set(FREETYPE ${LIB_DIR}/freetype/lib/freetype32.lib)

find_package(OpenGL REQUIRED)

add_subdirectory(Plutus)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

message("${CMAKE_CXX_COMPILER_ID}")

target_include_directories(Plutus PUBLIC ${LIB_DIR}/spdlog/include)

target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL} ${FREETYPE} Plutus opengl32)