cmake_minimum_required(VERSION 2.8.12)


project(e09-refactoring)

add_definitions("-std=c++11")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(
    PROJECT_SOURCES
    src/main.cpp
    src/Mesh.h
    src/Mesh.cpp
    src/Shader.h
    src/Shader.cpp
    src/Window.h
    src/Window.cpp
)

add_custom_target(
    copy-shaders ALL
    COMMENT "Copying shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders ${EXECUTABLE_OUTPUT_PATH}/shaders
)

add_custom_target(
    run
    COMMAND ${CMAKE_PROJECT_NAME}
    WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${CONAN_LIBS})
