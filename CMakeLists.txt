cmake_minimum_required(VERSION 3.26)
project(Logisim_clone LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_SUPPRESS_REGENERATION true)

find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)

file(GLOB SHADER_FILES "resources/shaders/*")
file(GLOB IMAGE_FILES "resources/images/*")
file(GLOB ENTITY_FILES "resources/entities/*")

file(COPY ${SHADER_FILES}
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/resources/shaders/")
file(COPY ${IMAGE_FILES}
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/resources/images/")
file(COPY ${ENTITY_FILES}
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/resources/entities/")

add_executable(Logisim_clone "Logisim(clone)/main.cpp")
target_include_directories(Logisim_clone PRIVATE "include/" "resources/")

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RELEASE_BUILD)
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC
                            RESOURCE_PATH="${CMAKE_CURRENT_BINARY_DIR}/resources/")

set_target_properties(Logisim_clone PROPERTIES 
                      XCODE_GENERATE_SCHEME TRUE
                      XCODE_SCHEME_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
target_link_libraries(Logisim_clone glfw GLEW::glew)
