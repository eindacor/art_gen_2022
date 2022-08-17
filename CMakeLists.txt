cmake_minimum_required(VERSION 3.23)
project(art_gen)

set(CMAKE_CXX_STANDARD 14)

add_compile_definitions(GLEW_STATIC)

# set root dir for precompiled glew/glfw (32 bit) binaries
set(GLEW_GLFW_PATH C:/dev)

include_directories(${GLEW_GLFW_PATH}/glfw-3.3.8.bin.WIN32/include)
link_directories(${GLEW_GLFW_PATH}/glfw-3.3.8.bin.WIN32/lib-mingw)

include_directories(${GLEW_GLFW_PATH}/glew-2.2.0/include)
link_directories(${GLEW_GLFW_PATH}/glew-2.2.0/lib/Release/Win32)

include_directories(${GLEW_GLFW_PATH}/glm/glm)


# This app requires an opengl boilerplate library called opengl_tools_2022, found below
# https://github.com/eindacor/opengl_tools_2022
#
# Clone that repo, build as a static library, and update the OGL_TOOLS_PROJECT_DIR var below with the repo path

set(OGL_TOOLS_PROJECT_DIR C:/Users/jpoll/CLionProjects/opengl_tools)
include_directories(${OGL_TOOLS_PROJECT_DIR})
link_directories(${OGL_TOOLS_PROJECT_DIR}/cmake-build-debug)



set(SOURCE_FILES main.cpp color_manager.cpp fractal_generator.cpp geometry_generator.cpp lens_shapes.cpp random_generator.cpp
        screencap.cpp settings_manager.cpp)

set(HEADERS color_manager.h fractal_generator.h geometry_generator.h header.h lens_shapes.h random_generator.h
        screencap.h settings_manager.h)

add_executable(art_gen ${SOURCE_FILES} ${HEADERS})

target_link_libraries(art_gen opengl_tools glew32s glfw3 glu32 opengl32)

add_custom_command(
        TARGET art_gen POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/PixelShader.glsl
        ${CMAKE_CURRENT_BINARY_DIR}/PixelShader.glsl)

add_custom_command(
        TARGET art_gen POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/VertexShader.glsl
        ${CMAKE_CURRENT_BINARY_DIR}/VertexShader.glsl)
