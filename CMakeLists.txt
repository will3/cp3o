cmake_minimum_required(VERSION 3.0)
project(cp3o)

find_package(OpenGL REQUIRED)

file(GLOB CP3O_SOURCE src/**/*.cpp src/*.cpp)

file(GLOB CP3O_HEADERS src/**/*.h src/*.h)

include_directories(
	src/core
	src/engine
	src/geometries
	src/postprocessing
	src/voxel
	../glew-2.1.0/include/
	../glm
	../glfw/include/
)

add_library(CP3O STATIC
	${CP3O_SOURCE}
	${CP3O_HEADERS}
)

target_link_libraries(CP3O
	${OPENGL_LIBRARY}
	GLEW
	glfw
)

file(COPY src/core/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY src/voxel/shaders DESTINATION ${CMAKE_BINARY_DIR})