#Install GLFW with cmake .. -DCMAKE_INSTALL_PREFIX=~/ -DBUILD_SHARED_LIBS=1

################################################################
# INITIALIZATION
################################################################
cmake_minimum_required(     VERSION 2.8)
set( CMAKE_BUILD_TYPE       "Release")
set( CMAKE_MACOSX_RPATH     1)
project( Minimal )
set( CMAKE_CXX_FLAGS        "-std=c++0x -O3 -w -lassimp")
include_directories(        "include/")
link_directories(           "/usr/local/lib/")

################################################################
#Dependencies
################################################################
find_package( OpenGL REQUIRED)
find_package( X11    REQUIRED)
find_package( ASSIMP REQUIRED)
if(ASSIMP_FOUND)
    include_directories(${ASSIMP_INCLUDE_DIR})
endif() 
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

if(APPLE)
	find_package( GLEW REQUIRED )
	find_package( PkgConfig REQUIRED)
	pkg_search_module(GLFW REQUIRED glfw3)
	include_directories(${GLFW_INCLUDE_DIRS})
	include_directories(${GLEW_INCLUDE_DIRS})
else()
	find_library( glew   NAMES GLEW    HINTS "$ENV{HOME}/lib")
	if(glew)
	  message(STATUS "Found GLEW: ${glew}")
	endif()
	find_library( glfw   NAMES glfw    HINTS "$ENV{HOME}/lib")
	if(glfw)
	  message(STATUS "Found GLFW: ${glfw}")
	endif()
endif()

if(APPLE)
	set(LIBS ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})
else()
	set(LIBS ${glfw} ${glew})
endif()
set(LIBS ${LIBS} ${FREETYPE_LIBRARIES} ${OPENGL_LIBRARY} ${X11_LIBRARIES} ${ASSIMP_LIBRARY})



################################################################
#Library
################################################################
file(GLOB_RECURSE source_files src/* include/*)
add_library(OGL SHARED ${source_files})
target_link_libraries(OGL ${LIBS} )

################################################################
#Executable
################################################################
add_executable(        cube main.cpp)
target_link_libraries( cube OGL)
install(TARGETS cube RUNTIME DESTINATION "/usr/local/bin")
